#include "Configuration.h"
#include <Windows.h>
#include <ShlObj.h>
#include <sstream>

using std::wstringstream;
using std::wstring;

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
		wstring file = Configuration::getIniFilePath();

		wchar_t buffer[BUFFER_SIZE];

		GetPrivateProfileStringW(L"SELECT_PTT_1", L"Device", L"", buffer, BUFFER_SIZE, file.c_str());
		configuration.selectPttOneDevice = wstring(buffer);
		configuration.selectPttOneButton = GetPrivateProfileIntW(L"SELECT_PTT_1", L"Button", 0, file.c_str());

		GetPrivateProfileStringW(L"SELECT_PTT_2", L"Device", L"", buffer, BUFFER_SIZE, file.c_str());
		configuration.selectPttTwoDevice = wstring(buffer);
		configuration.selectPttTwoButton = GetPrivateProfileIntW(L"SELECT_PTT_2", L"Button", 0, file.c_str());

		GetPrivateProfileStringW(L"SELECT_PTT_3", L"Device", L"", buffer, BUFFER_SIZE, file.c_str());
		configuration.selectPttThreeDevice = wstring(buffer);
		configuration.selectPttThreeButton = GetPrivateProfileIntW(L"SELECT_PTT_3", L"Button", 0, file.c_str());

		// Not in ini-file yet
		configuration.radioOnePan = -1.0f;
		configuration.radioTwoPan = 0.0f;
		configuration.radioThreePan = 1.0f;

		return configuration;
	}

	const wstring& Configuration::getSelectPttOneDevice() const
	{
		return this->selectPttOneDevice;
	}

	int Configuration::getSelectPttOneButton() const
	{
		return this->selectPttOneButton;
	}

	const wstring& Configuration::getSelectPttTwoDevice() const
	{
		return this->selectPttTwoDevice;
	}

	int Configuration::getSelectPttTwoButton() const
	{
		return this->selectPttTwoButton;
	}

	const wstring& Configuration::getSelectPttThreeDevice() const
	{
		return this->selectPttThreeDevice;
	}

	int Configuration::getSelectPttThreeButton() const
	{
		return this->selectPttThreeButton;
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

	wstring Configuration::getIniFilePath()
	{
		wchar_t* root = nullptr;
		SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &root);

		wstringstream stream;
		stream << root << L"\\MARS\\MARS.ini";
		CoTaskMemFree(static_cast<void*>(root));

		return stream.str();
	}
}
