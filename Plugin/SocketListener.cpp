#include "SocketListener.h"

namespace MARS
{
	SocketListener::SocketListener()
		: wsaData({ 0 })
		, serverSocket(INVALID_SOCKET)
	{
	}

	SocketListener::~SocketListener()
	{
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
		this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (this->serverSocket == INVALID_SOCKET)
		{
			// Fail
		}
		
		// Configure endpoint
		sockaddr_in endpoint = { 0 };
		endpoint.sin_family = AF_INET;
		endpoint.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		endpoint.sin_port = htons(2000);

		// Bind socket
		result = bind(this->serverSocket, (sockaddr*)&endpoint, sizeof(endpoint));
		if (result != 0)
		{
			// Fail
		}

		// Listen
		result = listen(this->serverSocket, SOMAXCONN);
		if (result == SOCKET_ERROR)
		{
			closesocket(this->serverSocket);
			WSACleanup();
		}
	}

	void SocketListener::Accept()
	{

	}
}