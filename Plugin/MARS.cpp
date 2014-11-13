#include "MARS.h"
#include "json/json.h"

namespace MARS
{
	const String PLUGIN_NAME = "MARS";
	const String PLUGIN_VERSION = "1.0 beta";
	const String PLUGIN_AUTHOR = "Master Arms";
	const String PLUGIN_DESCRIPTION = "MARS, Master Arms Radio System, integrates the radios in DCS World with TeamSpeak for a more realistic radio experience.";
	const String PLUGIN_KEYWORD = "mars";
	const int PLUGIN_API_LEVEL = 20;
	const unsigned short PORT = 2000;
	String pluginId = "";
	TS3Functions ts = { 0 };
	SocketListener listener;

	// Callback
	void OnMessageReceived(const char* message)
	{
		Json::Value root;
		Json::Reader reader;
		if (!reader.parse(message, root, false))
		{
			ts.printMessageToCurrentTab("JSON parsing of message failed");
		}
		else
		{
			std::string val = root.get("encoding", "default").asString();
			ts.printMessageToCurrentTab(val.c_str());
		}
	}
}
