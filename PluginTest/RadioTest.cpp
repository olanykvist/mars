#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Plugin/Radio.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using MARS::Radio;
using MARS::Modulation;
using MARS::Transmission;

namespace PluginTest
{		
	TEST_CLASS(RadioTest)
	{
	public:
		TEST_METHOD(DefaultConstructorShouldSetMembers)
		{
			Radio radio;
			Assert::IsTrue(radio.getPrimaryFrequency() == 0);
			Assert::IsTrue(radio.getSecondaryFrequency() == 0);
			Assert::IsTrue(radio.getPan() == 0.0f);
			Assert::IsTrue(radio.getModulation() == Modulation::AM);
			Assert::IsTrue(radio.getName() == "");
		}

		TEST_METHOD(GettersShouldReturnSetValue)
		{
			Radio radio;

			int primary = 121500000;
			radio.setPrimaryFrequency(primary);
			Assert::AreEqual(primary, radio.getPrimaryFrequency());

			int secondary = 225000000;
			radio.setSecondaryFrequency(secondary);
			Assert::AreEqual(secondary, radio.getSecondaryFrequency());

			float pan = 0.5f;
			radio.setPan(pan);
			Assert::AreEqual(pan, radio.getPan());

			radio.setModulation(Modulation::FM);
			Assert::IsTrue(radio.getModulation() == Modulation::FM);

			radio.setName("FR 22");
			Assert::AreEqual(radio.getName().c_str(), "FR 22");
		}

		TEST_METHOD(ShouldNotReceiveOnZero)
		{
			Radio radio;
			radio.setPrimaryFrequency(121500000);
			radio.setModulation(Modulation::AM);

			Assert::IsFalse(radio.canReceive(Transmission(0, Modulation::AM)));
		}

		TEST_METHOD(ShouldReceiveOnPrimary)
		{
			Radio radio;
			int frequency = 121500000;
			radio.setPrimaryFrequency(frequency);

			Assert::IsTrue(radio.canReceive(Transmission(frequency, Modulation::AM)));
		}
	};
}