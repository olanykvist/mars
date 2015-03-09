#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Plugin/Configuration.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using MARS::Configuration;
using std::wstring;

namespace PluginTest
{
	TEST_CLASS(ConfigurationTest)
	{
	public:
		TEST_METHOD(LoadConfiguration)
		{
			wstring empty;
			Configuration config = Configuration::load();

			Assert::AreNotEqual(empty, config.getSelectPttOneDevice());
			Assert::AreNotEqual(empty, config.getSelectPttTwoDevice());
			Assert::AreNotEqual(empty, config.getSelectPttThreeDevice());
		}
	};
}