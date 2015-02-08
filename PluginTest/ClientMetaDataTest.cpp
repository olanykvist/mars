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
			string s = data.serialize(false);
		}

		TEST_METHOD(Deserialize)
		{
			string document("{\"name\":\"Cool name\",\"radios\":[{\"frequency\":0,\"modulation\":0,\"name\":\"Off radio\"},{\"frequency\":127000000,\"modulation\":1,\"name\":\"AN - 65\"},{\"frequency\":0,\"modulation\":0,\"name\":\"init\"}],\"running\":false,\"selected\":0,\"unit\":\"MiG-21 bis\",\"version\":\"0.9999\"}");
			ClientMetaData data = ClientMetaData::deserialize(document);
		}
	};
}