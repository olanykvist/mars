#ifndef MARS_CONFIGURATION_H
#define MARS_CONFIGURATION_H

#include <string>

namespace MARS
{
	class Configuration
	{
	public:
		Configuration();
		virtual ~Configuration();
		static Configuration load();
		const std::wstring& getSelectPttOneDevice() const;
		int getSelectPttOneButton() const;
		const std::wstring& getSelectPttTwoDevice() const;
		int getSelectPttTwoButton() const;
		const std::wstring& getSelectPttThreeDevice() const;
		int getSelectPttThreeButton() const;
		float getRadioOnePan() const;
		float getRadioTwoPan() const;
		float getRadioThreePan() const;

	private:
		static std::wstring getIniFilePath();
		std::wstring selectPttOneDevice;
		int selectPttOneButton;
		std::wstring selectPttTwoDevice;
		int selectPttTwoButton;
		std::wstring selectPttThreeDevice;
		int selectPttThreeButton;
		float radioOnePan;
		float radioTwoPan;
		float radioThreePan;
	};
};

#endif
