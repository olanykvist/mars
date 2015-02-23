#ifndef MARS_CLIENTMETADATA_H
#define MARS_CLIENTMETADATA_H

#include <string>
#include "Modulation.h"
#include "Vector3.h"

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
		std::string serialize(bool formatted = false) const;
		static const ClientMetaData deserialize(const std::string& document);

	public:
		std::string version;
		bool running;
		std::string name;
		std::string unit;
		int selected;
		Vector3 position;
		RadioInformation radio[3];
	};
};

#endif
