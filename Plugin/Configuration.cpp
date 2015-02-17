#include "Configuration.h"

namespace MARS
{
	Configuration::Configuration()
		: selectPttOneDevice()
		, selectPttOneButton(0)
		, selectPttTwoDevice()
		, selectPttTwoButton(0)
		, selectPttThreeDevice()
		, selectPttThreeButton(0)
		, radioOnePan(0.0f)
		, radioTwoPan(0.0f)
		, radioThreePan(0.0f)
	{
	}

	Configuration::~Configuration()
	{
	}

	Configuration Configuration::load()
	{
		Configuration configuration;
		return configuration;
	}

	const std::string& Configuration::getSelectPttOneDevice() const
	{
		return this->selectPttOneDevice;
	}

	int Configuration::getSelectPttOneButton() const
	{
		return this->selectPttOneButton;
	}

	const std::string& Configuration::getSelectPttTwoDevice() const
	{
		return this->selectPttTwoDevice;
	}

	int Configuration::getSelectPttTwoButton() const
	{
		return this->selectPttTwoButton;
	}

	const std::string& Configuration::getSelectPttThreeDevice() const
	{
		return this->selectPttThreeDevice;
	}

	int Configuration::getSelectPttThreeButton() const
	{
		return this->selectPttTwoButton;
	}

	float Configuration::getRadioOnePan() const
	{
		return this->radioOnePan;
	}

	float Configuration::getRadioTwoPan() const
	{
		return this->radioTwoPan;
	}

	float Configuration::getRadioThreePan() const
	{
		return this->radioThreePan;
	}
}
