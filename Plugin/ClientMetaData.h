#ifndef MARS_CLIENTMETADATA_H
#define MARS_CLIENTMETADATA_H

#include <string>
#include "Modulation.h"

namespace MARS
{
	struct RadioInformation
	{
		std::string name;
		int frequency;
		Modulation modulation;
	};

	class ClientMetaData
	{
	public:
		ClientMetaData();
		std::string serialize() const;

	private:
		std::string version;
		bool isRunning;
		std::string player;
		std::string type;
		RadioInformation radio[3];
		int selected;
	};
};

#endif
