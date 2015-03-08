#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Plugin/InputListener.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using MARS::InputListener;

namespace PluginTest
{
	TEST_CLASS(InputListenerTest)
	{
	public:
		TEST_METHOD(ShouldStartAndStopWithoutCrash)
		{
			InputListener* listener = new InputListener();
			listener->Start();

			delete listener;
		}
	};
}