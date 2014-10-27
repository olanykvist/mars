#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Plugin/Radio.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using MARS::Radio;

namespace PluginTest
{		
	TEST_CLASS(RadioTest)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			Radio radio;
			Assert::AreEqual(1, 1);
		}
	};
}