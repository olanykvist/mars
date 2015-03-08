#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Plugin/SoundPlayer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using MARS::SoundPlayer;

namespace PluginTest
{
	TEST_CLASS(SoundPlayerTest)
	{
	public:
		TEST_METHOD(ShouldPlaySound)
		{
			SoundPlayer player;
			try
			{
				player.Initialize();
				player.Load("C:\\Users\\Ola\\Desktop\\ptt_up.raw");
			}
			catch (char* error)
			{

			}

			player.Play("C:\\Users\\Ola\\Desktop\\ptt_up.raw");
		}
	};
}