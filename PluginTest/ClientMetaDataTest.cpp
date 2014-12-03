#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Plugin/ClientMetaData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using MARS::ClientMetaData;
using std::string;

namespace PluginTest
{
	TEST_CLASS(ClientMetaDataTest)
	{
	public:
		TEST_METHOD(Serialize)
		{
			ClientMetaData data;
			string s = data.serialize();
		}
	};
}