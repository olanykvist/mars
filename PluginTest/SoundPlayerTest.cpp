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
				player.Load("ptt_up.raw");
			}
			catch (...)
			{

			}

			player.Play("ptt_up.raw", -1.0f);
			Sleep(1000);
			player.Play("ptt_up.raw", -0.5f);
			Sleep(1000);
			player.Play("ptt_up.raw", 0.0f);
			Sleep(1000);
			player.Play("ptt_up.raw", 0.5f);
			Sleep(1000);
			player.Play("ptt_up.raw", 1.0f);
			Sleep(1000);

			for (int i = 0; i < 10; ++i)
			{
				float vol = i * 0.1f;
				player.Play("ptt_up.raw", 0.0f, vol);
				Sleep(500);
			}
		}
	};
}