#include "SocketListener.h"
#include <thread>

using std::thread;

namespace MARS
{
	SocketListener::SocketListener()
		: listening(false)
		, listener(INVALID_SOCKET)
		, connections()
		, acceptor()
		, MessageReceived(nullptr)
	{}

	SocketListener::~SocketListener()
	{
		if (this->listening)
		{
			this->Stop();
		}
	}

	void SocketListener::Start(unsigned short port)
	{
		WSAData data;

		int result = WSAStartup(WINSOCK_VERSION, &data);
		if (result != 0)
		{
			// Fail
		}

		this->listener = this->CreateListenSocket(port);
		if (this->listener == INVALID_SOCKET)
		{
			throw;
		}

		this->listening = true;
		this->acceptor = thread(&SocketListener::AcceptConnections, this, this->listener);
	}

	void SocketListener::Stop()
	{
		this->listening = false;

		closesocket(listener);
		if (this->acceptor.joinable())
		{
			this->acceptor.join();
		}

		int result = WSACleanup();
		if (result != 0)
		{
			//cout << "WSACleanup failed" << endl;
		}
	}

	SOCKET SocketListener::CreateListenSocket(unsigned short port)
	{
		SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listener != INVALID_SOCKET)
		{
			sockaddr_in endpoint = { 0 };
			endpoint.sin_family = AF_INET;
			endpoint.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
			endpoint.sin_port = htons(port);

			int result = bind(listener, (sockaddr*)&endpoint, sizeof(endpoint));
			if (result != SOCKET_ERROR)
			{
				listen(listener, SOMAXCONN);
				return listener;
			}
		}

		return INVALID_SOCKET;
	}

	void SocketListener::SetupFileDescriptorSets(fd_set& readfds, fd_set& writefds, fd_set& exceptfds, SOCKET listener)
	{
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);

		if (listener != INVALID_SOCKET)
		{
			FD_SET(listener, &readfds);
			FD_SET(listener, &exceptfds);
		}

		for (auto& connection : connections)
		{
			FD_SET(connection.socket, &readfds);
			FD_SET(connection.socket, &writefds);
			FD_SET(connection.socket, &exceptfds);
		}
	}

	void SocketListener::AcceptConnections()
	{
		sockaddr_in endpoint;
		int endpointSize = sizeof(endpoint);

		while (this->listening)
		{
			fd_set readFDs, writeFDs, exceptFDs;
			this->SetupFileDescriptorSets(readFDs, writeFDs, exceptFDs, this->listener);

			if (select(0, &readFDs, &writeFDs, &exceptFDs, 0) > 0)
			{
				// Listener socket?
				if (FD_ISSET(this->listener, &readFDs))
				{
					SOCKET sd = accept(this->listener, (sockaddr*)&endpoint, &endpointSize);
					if (sd != INVALID_SOCKET)
					{
						// Connection accepted
						connections.push_back(Connection(sd));

						// Set "no block" mode on socket
						u_long noBlock = 1;
						ioctlsocket(sd, FIONBIO, &noBlock);
					}
					else
					{
						// "accept() failed"
					}
				}

				// Exception on listener socket
				else if (FD_ISSET(this->listener, &exceptFDs))
				{
					// Exception on listening socket
				}

				// Or one of clients?
				auto i = connections.begin();
				while (i != connections.end())
				{
					bool ok = true;
					const char* error = nullptr;

					if (FD_ISSET(i->socket, &exceptFDs))
					{
						ok = false;
						error = "General socket error";
						FD_CLR(i->socket, &exceptFDs);
					}
					else
					{
						if (FD_ISSET(i->socket, &readFDs))
						{
							ok = this->ReadData(*i);
							error = "Read error";
							FD_CLR(i->socket, &readFDs);
						}
						if (FD_ISSET(i->socket, &writeFDs))
						{
							// Writing not implemented...
							FD_CLR(i->socket, &writeFDs);
						}
					}

					if (!ok)
					{
						// Something bad happened on the socket, or the
						// client closed its half of the connection.  Shut
						// the conn down and remove it from the list.
						int err;
						int errlen = sizeof(err);
						getsockopt(i->socket, SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
						if (err != NO_ERROR)
						{
							//std::cout << WSAGetLastErrorMessage(error, err) << std::endl;
							//std::cout << "Error";
						}
						ShutdownConnection(*i);
						connections.erase(i);
						i = connections.begin();
					}
					else
					{
						++i;
					}
				}
			}
			else
			{
				// select() failed
			}
		}

	}

	bool SocketListener::ReadData(Connection& connection)
	{
		memset(connection.buffer, 0, Connection::BUFFER_SIZE);
		int count = recv(connection.socket, connection.buffer, Connection::BUFFER_SIZE, 0);

		if (count == 0)
		{
			// Connection closed by client
			return false;
		}
		else if (count == SOCKET_ERROR)
		{
			// Something bad happened on the socket.
			int err;
			int errlen = sizeof(err);
			getsockopt(connection.socket, SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
			return (err == WSAEWOULDBLOCK);
		}

		if (this->MessageReceived != nullptr)
		{
			this->MessageReceived(connection.buffer);
		}

		return true;
	}

	bool SocketListener::ShutdownConnection(Connection& connection)
	{
		// Disallow any further data sends.  This will tell the other side
		// that we want to go away now.  If we skip this step, we don't
		// shut the connection down nicely.
		if (shutdown(connection.socket, SD_SEND) == SOCKET_ERROR)
		{
			return false;
		}

		// Receive any extra data still sitting on the socket.  After all
		// data is received, this call will block until the remote host
		// acknowledges the TCP control packet sent by the shutdown above.
		// Then we'll get a 0 back from recv, signalling that the remote
		// host has closed its side of the connection.
		char buffer[Connection::BUFFER_SIZE];
		while (true)
		{
			int count = recv(connection.socket, buffer, Connection::BUFFER_SIZE, 0);
			if (count == SOCKET_ERROR)
			{
				return false;
			}
			else if (count != 0)
			{
				//std::cout << std::endl << "FYI, received " << count << " unexpected bytes during shutdown." << std::endl;
			}
			else
			{
				// Okay, we're done!
				break;
			}
		}

		// Close the socket.
		if (closesocket(connection.socket) == SOCKET_ERROR)
		{
			return false;
		}

		return true;
	}
}