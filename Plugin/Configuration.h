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
		const std::string& getSelectPttOneDevice() const;
		int getSelectPttOneButton() const;
		const std::string& getSelectPttTwoDevice() const;
		int getSelectPttTwoButton() const;
		const std::string& getSelectPttThreeDevice() const;
		int getSelectPttThreeButton() const;
		float getRadioOnePan() const;
		float getRadioTwoPan() const;
		float getRadioThreePan() const;

	private:
		static std::string getIniFilePath();
		std::string selectPttOneDevice;
		int selectPttOneButton;
		std::string selectPttTwoDevice;
		int selectPttTwoButton;
		std::string selectPttThreeDevice;
		int selectPttThreeButton;
		float radioOnePan;
		float radioTwoPan;
		float radioThreePan;
	};
};

#endif
