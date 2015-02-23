#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Plugin/SocketListener.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using MARS::SocketListener;

namespace PluginTest
{
	void OnMessage(const char* message)
	{

	}

	TEST_CLASS(SocketListenerTest)
	{
	public:
		TEST_METHOD(ShouldStartAndStopWithoutCrash)
		{
			SocketListener listener;
			listener.Start(10112);
			Sleep(5000);
			listener.Stop();
		}
	};
}