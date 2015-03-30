#include "ClientMetaData.h"
#include "json/json.h"

using std::string;

namespace MARS
{
	ClientMetaData::ClientMetaData()
		: version("init")
		, running(false)
		, name("init")
		, unit("init")
		, selected(0)
		, position()
	{
		for (size_t i = 0; i < 3; i++)
		{
			this->radio[i].name = "init";
			this->radio[i].frequency = 0;
			this->radio[i].modulation = Modulation::AM;
		}
	}

	string ClientMetaData::serialize(bool formatted) const
	{
		Json::Value root;
		root["version"] = this->version;
		root["running"] = this->running;
		root["name"] = this->name;
		root["unit"] = this->unit;
		root["selected"] = this->selected;

		Json::Value array;
		for (int i = 0; i < 3; i++)
		{
			Json::Value current;
			current["name"] = this->radio[i].name;
			current["frequency"] = this->radio[i].frequency;
			current["modulation"] = this->radio[i].modulation;
			array.append(current);
		}

		root["radios"] = array;

		Json::Value position;
		position["x"] = this->position.x;
		position["y"] = this->position.y;
		position["z"] = this->position.z;

		root["pos"] = position;

		if (formatted == true)
		{
			Json::StyledWriter writer;
			return writer.write(root);
		}
		else
		{
			Json::FastWriter writer;
			return writer.write(root);
		}
	}

	const ClientMetaData ClientMetaData::deserialize(const string& document)
	{
		ClientMetaData data;
		Json::Reader reader;
		Json::Value root;
		
		bool success = reader.parse(document, root, false);
		if (success == true)
		{
			data.version = root["version"].asString();
			data.running = root["running"].asBool();
			data.name = root["name"].asString();
			data.unit = root["unit"].asString();
			data.selected = root["selected"].asInt();

			data.position.x = root["pos"]["x"].asFloat();
			data.position.y = root["pos"]["y"].asFloat();
			data.position.z = root["pos"]["z"].asFloat();

			for (int i = 0; i < 3; i++)
			{
				data.radio[i].name = root["radios"][i]["name"].asString();
				data.radio[i].frequency = root["radios"][i]["frequency"].asInt();
				data.radio[i].modulation = (MARS::Modulation)root["radios"][i]["modulation"].asInt();
			}
		}
		else
		{
			throw string("Failed to parse metadata. Conflicting plugin?");
		}

		return data;
	}
}