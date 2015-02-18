#include "Configuration.h"
#include <Windows.h>
#include <ShlObj.h>
#include <sstream>
#include <locale>

using std::wstringstream;
using std::string;

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
		const size_t BUFFER_SIZE = 128;

		Configuration configuration;
		string file = Configuration::getIniFilePath();

		char buffer[BUFFER_SIZE];

		GetPrivateProfileStringA("SELECT_PTT_1", "Device", "", buffer, BUFFER_SIZE, file.c_str());
		configuration.selectPttOneDevice = string(buffer);
		configuration.selectPttOneButton = GetPrivateProfileIntA("SELECT_PTT_1", "Button", 0, file.c_str());

		GetPrivateProfileStringA("SELECT_PTT_2", "Device", "", buffer, BUFFER_SIZE, file.c_str());
		configuration.selectPttTwoDevice = string(buffer);
		configuration.selectPttTwoButton = GetPrivateProfileIntA("SELECT_PTT_2", "Button", 0, file.c_str());

		GetPrivateProfileStringA("SELECT_PTT_3", "Device", "", buffer, BUFFER_SIZE, file.c_str());
		configuration.selectPttThreeDevice = string(buffer);
		configuration.selectPttThreeButton = GetPrivateProfileIntA("SELECT_PTT_3", "Button", 0, file.c_str());

		// Not in ini-file yet
		configuration.radioOnePan = -1.0f;
		configuration.radioTwoPan = 0.0f;
		configuration.radioThreePan = 1.0f;

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

	std::string Configuration::getIniFilePath()
	{
		wchar_t* root = nullptr;
		SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &root);

		wstringstream stream;
		stream << root << L"\\MARS\\MARS.ini";
		std::wstring ws = stream.str();
		CoTaskMemFree(static_cast<void*>(root));

		std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>> converter;
		std::string path = converter.to_bytes(ws);

		return path;
	}
}
