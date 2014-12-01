#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Plugin/SocketListener.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using MARS::SocketListener;

namespace PluginTest
{
	TEST_CLASS(SocketListenerTest)
	{
	public:
		TEST_METHOD(ShouldStartAndStopWithoutCrash)
		{
			SocketListener listener;
			listener.Initialize();
			listener.Start();

			Sleep(5000);

			// Check of port is open
			// Try to connect

			listener.Stop();
			listener.Destroy();
		}
	};
}