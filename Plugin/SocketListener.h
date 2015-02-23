#ifndef MARS_SOCKETLISTENER_H
#define MARS_SOCKETLISTENER_H

#include <WinSock2.h>
#include <vector>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

namespace MARS
{
	struct Connection
	{
		explicit Connection(SOCKET s) : socket(s) {};
		static const size_t BUFFER_SIZE = 512;
		SOCKET socket;
		char buffer[Connection::BUFFER_SIZE];
	};

	class SocketListener
	{
	public:
		typedef void(*OnMessageReceived)(const char*);
		SocketListener();
		~SocketListener();
		void Start(unsigned short port);
		void Stop();
		OnMessageReceived MessageReceived;

	private:
		bool listening;
		SOCKET listener;
		std::vector<MARS::Connection> connections;
		std::thread acceptor;
		SOCKET CreateListenSocket(unsigned short port);
		void SetupFileDescriptorSets(fd_set& readfds, fd_set& writefds, fd_set& exceptfds, SOCKET listener = INVALID_SOCKET);
		void AcceptConnections();
		bool ReadData(Connection& connection);
		bool ShutdownConnection(Connection& connection);
	};

	//class SocketListener
	//{
	//public:
	//	typedef void(*OnMessage)(const char*);
	//	SocketListener();
	//	~SocketListener();
	//	void Initialize();
	//	void Start();
	//	void Stop();
	//	void Destroy();
	//	OnMessage onMessage;

	//private:
	//	bool isListening;
	//	WSADATA wsaData;
	//	SOCKET listenSocket;
	//	vector<SOCKET> clientSockets;
	//	thread listenThread;
	//	vector<thread> receiveThreads;
	//	void AcceptConnections();
	//	void HandleConnection(SOCKET client);
	//};
};

#endif
