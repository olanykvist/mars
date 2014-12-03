#include "ClientMetaData.h"
#include "json/json.h"

using std::string;

namespace MARS
{
	ClientMetaData::ClientMetaData()
		: version("beta")
		, isRunning(false)
		, player("init")
		, type("init")
		, selected(0)
	{
		for (size_t i = 0; i < 3; i++)
		{
			this->radio[i].name = "init";
			this->radio[i].frequency = 0;
			this->radio[i].modulation = Modulation::AM;
		}
	}

	string ClientMetaData::serialize() const
	{
		Json::Value root;
		root["version"] = this->version;
		root["running"] = this->isRunning;
		root["player"] = this->player;
		root["type"] = this->type;
		root["selected"] = this->selected;

		Json::StyledWriter writer;
		return writer.write(root);
	}
}