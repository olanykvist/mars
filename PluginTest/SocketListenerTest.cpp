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
		TEST_METHOD(TestMethod1)
		{
			SocketListener listener;
			listener.Initialize();
		}
	};
}