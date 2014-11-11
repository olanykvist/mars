#ifndef MARS_SOCKETLISTENER_H
#define MARS_SOCKETLISTENER_H

#include <WinSock2.h>
#include <vector>
#include <thread>

using std::vector;
using std::thread;

#pragma comment(lib, "Ws2_32.lib")

namespace MARS
{
	class SocketListener
	{
	public:
		typedef void(*OnMessage)(const char*);
		SocketListener();
		~SocketListener();
		void Initialize();
		void Start();
		void Stop();
		void Destroy();
		OnMessage onMessage;

	private:
		bool isListening;
		WSADATA wsaData;
		SOCKET listenSocket;
		vector<SOCKET> clientSockets;
		thread listenThread;
		vector<thread> receiveThreads;
		void AcceptConnections();
		void HandleConnection(SOCKET client);
	};
};

#endif
