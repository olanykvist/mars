#ifndef MARS_SOCKETLISTENER_H
#define MARS_SOCKETLISTENER_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

namespace MARS
{
	class SocketListener
	{
	public:
		typedef int(MESSAGE_RECEIVED)();
		SocketListener();
		virtual ~SocketListener();
		void Initialize();
		void Accept();

	private:
		WSADATA wsaData;
		SOCKET serverSocket;
		MESSAGE_RECEIVED OnMessageReceived;
	};
};

#endif
