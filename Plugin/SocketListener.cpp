#include "SocketListener.h"
#include <thread>

namespace MARS
{
	SocketListener::SocketListener()
		: wsaData({ 0 })
		, listenSocket(INVALID_SOCKET)
		, isListening(false)
	{
	}

	SocketListener::~SocketListener()
	{
		this->Destroy();
	}

	void SocketListener::Initialize()
	{
		// Initialize WinSock
		int result = WSAStartup(MAKEWORD(2, 2), &(this->wsaData));
		if (result != 0)
		{
			// Fail
		}

		// Create socket
		this->listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (this->listenSocket == INVALID_SOCKET)
		{
			// Fail
		}

		// Configure endpoint
		sockaddr_in endpoint = { 0 };
		endpoint.sin_family = AF_INET;
		endpoint.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		endpoint.sin_port = htons(2000);

		// Bind socket
		result = bind(this->listenSocket, (sockaddr*)&endpoint, sizeof(endpoint));
		if (result != 0)
		{
			// Fail
		}

		// Listen
		result = listen(this->listenSocket, SOMAXCONN);
		if (result == SOCKET_ERROR)
		{
			closesocket(this->listenSocket);
			WSACleanup();
		}
	}

	void SocketListener::Start()
	{
		this->isListening = true;
		this->listenThread = std::thread(&SocketListener::AcceptConnections, this);
	}

	void SocketListener::Stop()
	{
		this->isListening = false;

		// Close all client connections in a somewhat nice way
		for (auto& s : this->clientSockets)
		{
			const size_t BUFFER_SIZE = 256;
			char buffer[BUFFER_SIZE];
			memset(buffer, 0, BUFFER_SIZE);
			while (recv(s, buffer, BUFFER_SIZE, 0) > 0);
			closesocket(s);
		}
		this->clientSockets.clear();

		for (auto& t : this->receiveThreads)
		{
			if (t.joinable())
			{
				t.join();
			}
		}
		this->receiveThreads.clear();

		// Close listener socket (accept() will return with error)
		closesocket(this->listenSocket);
		if (this->listenThread.joinable())
		{
			this->listenThread.join();
		}
	}

	void SocketListener::Destroy()
	{
		this->Stop();
		WSACleanup();
	}

	void SocketListener::AcceptConnections()
	{
		SOCKET client = INVALID_SOCKET;
		while (this->isListening)
		{
			client = accept(this->listenSocket, nullptr, nullptr);
			if (client != INVALID_SOCKET)
			{
				this->clientSockets.push_back(client);
				this->receiveThreads.push_back(thread(&SocketListener::HandleConnection, this, client));
			}
		}
	}

	void SocketListener::HandleConnection(SOCKET client)
	{
		const size_t BUFFER_SIZE = 256;
		char buffer[BUFFER_SIZE];
		int count = 0;

		do
		{
			memset(buffer, 0, BUFFER_SIZE);
			count = recv(client, buffer, BUFFER_SIZE, 0);
			if (count > 0)
			{
				// buffer holds received message
			}
			else if (count == 0)
			{
				// Client closed socket
			}
			else
			{
				// Error reading from socket
				int error = WSAGetLastError();
			}
		} while (count > 0);
	}
}