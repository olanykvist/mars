// Standard includes

// TeamSpeak SDK includes
#include "public_errors.h"
#include "public_errors_rare.h"
#include "public_definitions.h"
#include "public_rare_definitions.h"
#include "ts3_functions.h"
#include "Plugin.h"
#include "ClientMetaData.h"
#include "Transmission.h"
#include "json/json.h"

using std::string;

static MARS::Plugin plugin;

namespace MARS
{
	const char* Plugin::NAME = "MARS";
	const char* Plugin::VERSION = "1.0 beta";
	const char* Plugin::AUTHOR = "Master Arms";
	const char* Plugin::DESCRIPTION = "MARS, Master Arms Radio System, integrates the radios in DCS World with TeamSpeak for a more realistic radio experience.";
	const char* Plugin::COMMAND_KEYWORD = "mars";
	const int   Plugin::API_VERSION = 20;

	Plugin::Plugin()
		: teamspeak({ 0 })
		, pluginId(nullptr)
		, usingVAD(false)
		, inGame(false)
		, usingExternal(false)
		, selectedRadioIndex(0)
		, internal()
		, external()
		, currentRadio(nullptr)
		, socketListener()
		, inputListener()
		, metaData()
		, position()
		, configuration()
		, receivers()
	{
		// Add external radios
		this->external.push_back(Radio());
		this->external.push_back(Radio());
		this->external.push_back(Radio());

		// Add internal/DCS radios
		this->internal.push_back(Radio());
		this->internal.push_back(Radio());
		this->internal.push_back(Radio());

		//
		this->external[0].setName("EXT 1");
		this->external[1].setName("EXT 2");
		this->external[2].setName("EXT 3");
	}

	Plugin::~Plugin()
	{
		if (this->pluginId)
		{
			delete[] this->pluginId;
		}
	}

	void Plugin::setTeamSpeakFunctions(TS3Functions functions)
	{
		this->teamspeak = functions;
	}

	void Plugin::setPluginId(const char* id)
	{
		size_t len = strlen(id);
		this->pluginId = new char[len + 1];
		strcpy_s(this->pluginId, len + 1, id);
	}

	bool Plugin::processCommand(uint64 serverConnectionHandlerId, const char* command)
	{
		if (strcmp(command, "dump") == 0)
		{
			anyID id;
			this->teamspeak.getClientID(serverConnectionHandlerId, &id);
			string data = this->getClientMetaData(serverConnectionHandlerId, id);
			this->teamspeak.printMessageToCurrentTab(data.c_str());
			return true;
		}

		return false;
	}

	string Plugin::getClientInfoData(uint64 serverConnectionHandlerId, uint64 clientId) const
	{
		const size_t BUFFER_SIZE = 256;
		const int MHz = 1000000;
		char buffer[BUFFER_SIZE] = { 0 };

		string metadata = this->getClientMetaData(serverConnectionHandlerId, clientId);

		if (metadata.length() == 0)
		{
			return "Not installed";
		}

		ClientMetaData client;
		try
		{
			client = ClientMetaData::deserialize(metadata);
		}
		catch (...)
		{
			return "Failed to parse metadata. Conflicting plugin?";
		}

		sprintf_s(buffer, BUFFER_SIZE, "Installed (version %s)\n", client.version.c_str());

		if (client.running)
		{
			char status[128] = { 0 };
			sprintf_s(status, 128, "Status: In-game as %s, playing %s\n", client.name.c_str(), client.unit.c_str());
			strcat_s(buffer, BUFFER_SIZE, status);

			char pos[128] = { 0 };
			sprintf_s(pos, 128, "X%d Y%d Z%d\n", (int)client.position.x, (int)client.position.y, (int)client.position.z);
			strcat_s(buffer, BUFFER_SIZE, pos);

			for (int i = 0; i < 3; i++)
			{
				if (client.radio[i].name == "N/A")
				{
					continue;
				}

				char radio[128] = { 0 };
				float frequency = (float)client.radio[i].frequency / MHz;

				sprintf_s(radio, 128, "%s - %3.3f ", client.radio[i].name.c_str(), frequency);

				if (client.radio[i].modulation == AM)
				{
					strcat_s(radio, 128, "AM");
				}
				else if (client.radio[i].modulation == FM)
				{
					strcat_s(radio, 128, "FM");
				}

				if ((client.selected - 1) == i)
				{
					strcat_s(radio, 128, "[B]*[/B]");
				}

				strcat_s(radio, 128, "\n");
				strcat_s(buffer, BUFFER_SIZE, radio);
			}
		}
		else
		{
			strcat_s(buffer, BUFFER_SIZE, "Status: Not in-game\n");
		}

		return buffer;
	}

	string Plugin::getClientMetaData(uint64 serverConnectionHandlerId, uint64 clientId) const
	{
		string data;
		char* result;
		if (this->teamspeak.getClientVariableAsString(serverConnectionHandlerId, (anyID)clientId, CLIENT_META_DATA, &result) == ERROR_ok)
		{
			data = string(result);
			this->teamspeak.freeMemory(result);
		}

		return data;
	}

	void Plugin::initListener()
	{
		this->socketListener.MessageReceived = Plugin::onMessageReceived;
		this->socketListener.Start(10112);
	}

	void Plugin::shutdownListener()
	{
		this->socketListener.Stop();
	}

	void Plugin::initInput()
	{
		this->inputListener.ButtonDown = Plugin::onButtonDown;
		this->inputListener.ButtonUp = Plugin::onButtonUp;
		this->inputListener.Start();
	}

	void Plugin::shutdownInput()
	{
		this->inputListener.Stop();
	}

	void Plugin::loadConfiguration()
	{
		this->configuration = Configuration::load();
	}

	// Callback
	void Plugin::onClientUpdated(uint64 serverConnectionHandlerId, anyID clientId, anyID invokerId)
	{
		if (this->inGame == false)
		{
			return;
		}

		anyID myId = 0;
		if (this->teamspeak.getClientID(serverConnectionHandlerId, &myId) != ERROR_ok)
		{
			this->teamspeak.logMessage("Failed to get own Id", LogLevel_ERROR, Plugin::NAME, 0);
			return;
		}


		uint64 channelId = 0;
		if (this->teamspeak.getChannelOfClient(serverConnectionHandlerId, myId, &channelId) != ERROR_ok)
		{
			this->teamspeak.logMessage("Failed to get channel id", LogLevel_ERROR, Plugin::NAME, 0);
			return;
		}

		anyID* clients;
		if (this->teamspeak.getChannelClientList(serverConnectionHandlerId, channelId, &clients) != ERROR_ok)
		{
			this->teamspeak.logMessage("Failed to get clients in channel", LogLevel_ERROR, Plugin::NAME, 0);
			return;
		}

		this->receivers.clear();

		for (int i = 0; clients[i] != 0; i++)
		{
			string json = this->getClientMetaData(serverConnectionHandlerId, clients[i]);
			ClientMetaData data = ClientMetaData::deserialize(json);
			Transmission transmission = Transmission(data.radio[data.selected - 1].frequency, data.radio[data.selected - 1].modulation);

			this->receivers[clients[i]] = nullptr;

			if (this->usingExternal)
			{
				for (auto& radio : external)
				{
					if (radio.canReceive(transmission))
					{
						this->receivers[clients[i]] = &radio;
					}
				}
			}
			else
			{
				for (auto& radio : internal)
				{
					if (radio.canReceive(transmission))
					{
						this->receivers[clients[i]] = &radio;
					}
				}
			}
		}
	}

	void Plugin::onMessageReceived(const char* message)
	{
		// Debug output
		plugin.teamspeak.printMessageToCurrentTab(message);

		string json(message);
		Json::Reader reader;
		Json::Value root;

		bool success = reader.parse(json, root, false);
		if (success)
		{
			string command = root["command"].asString();

			if (command == "set")
			{
				bool internal = root["internal"].asBool();
				string name = root["name"].asString();
				int primary = root["primary"].asInt();
				int secondary = root["secondary"].asInt();
				int radio = root["radio"].asInt();
				Modulation modulation = (Modulation)root["modulation"].asInt();

				plugin.setRadioInformation(internal, name, radio, primary, secondary, modulation);
			}
			else if (command == "pos")
			{
				float x = root["x"].asFloat();
				float y = root["y"].asFloat();
				float z = root["z"].asFloat();

				plugin.setPosition(x, y, z);
			}
			else if (command == "select")
			{
				int id = root["radio"].asInt();

				plugin.selectActiveRadio(id);
			}
			else if (command == "use")
			{
				string mode = root["mode"].asString();

				if (mode == "internal")
				{
					plugin.useInternalRadios();
				}
				else
				{
					plugin.useExternalRadios();
				}
			}
			else if (command == "info")
			{
				string name = root["name"].asString();
				string unit = root["unit"].asString();

				plugin.setPlayerInformation(name, unit);
			}
			else if (command == "start")
			{
				plugin.start();
			}
			else if (command == "stop")
			{
				plugin.stop();
			}
		}
		else
		{
			if (json != "poke")
			{
				string error = "Failed to parse command: " + json;
				plugin.teamspeak.logMessage(error.c_str(), LogLevel_ERROR, "MARS", 0);
			}
		}
	}

	void Plugin::onButtonDown(const wchar_t* device, int button)
	{
		if (device == plugin.configuration.getSelectPttOneDevice())
		{
			if (button == plugin.configuration.getSelectPttOneButton())
			{
				plugin.teamspeak.printMessageToCurrentTab("PTT 1 DOWN");
			}
		}

		if (device == plugin.configuration.getSelectPttTwoDevice())
		{
			if (button == plugin.configuration.getSelectPttTwoButton())
			{
				plugin.teamspeak.printMessageToCurrentTab("PTT 2 DOWN");
			}
		}

		if (device == plugin.configuration.getSelectPttThreeDevice())
		{
			if (button == plugin.configuration.getSelectPttThreeButton())
			{
				plugin.teamspeak.printMessageToCurrentTab("PTT 3 DOWN");
			}
		}
	}

	void Plugin::onButtonUp(const wchar_t* device, int button)
	{
		if (device == plugin.configuration.getSelectPttOneDevice())
		{
			if (button == plugin.configuration.getSelectPttOneButton())
			{
				plugin.teamspeak.printMessageToCurrentTab("PTT 1 UP");
			}
		}

		if (device == plugin.configuration.getSelectPttTwoDevice())
		{
			if (button == plugin.configuration.getSelectPttTwoButton())
			{
				plugin.teamspeak.printMessageToCurrentTab("PTT 2 UP");
			}
		}

		if (device == plugin.configuration.getSelectPttThreeDevice())
		{
			if (button == plugin.configuration.getSelectPttThreeButton())
			{
				plugin.teamspeak.printMessageToCurrentTab("PTT 3 UP");
			}
		}
	}

	void Plugin::updateMetaData(bool flush)
	{
		this->metaData.version = Plugin::VERSION;
		this->metaData.running = this->inGame;
		this->metaData.name = this->name;
		this->metaData.unit = this->unit;
		this->metaData.selected = this->selectedRadioIndex + 1;
		this->metaData.position = this->position;

		for (int i = 0; i < 3; i++)
		{
			if (this->usingExternal)
			{
				this->metaData.radio[i].name = this->external.at(i).getName();
				this->metaData.radio[i].frequency = this->external.at(i).getPrimaryFrequency();
				this->metaData.radio[i].modulation = this->external.at(i).getModulation();
			}
			else
			{
				this->metaData.radio[i].name = this->internal.at(i).getName();
				this->metaData.radio[i].frequency = this->internal.at(i).getPrimaryFrequency();
				this->metaData.radio[i].modulation = this->internal.at(i).getModulation();
			}
		}

		string data = this->metaData.serialize();

		uint64 connection = this->teamspeak.getCurrentServerConnectionHandlerID();
		if (!connection)
		{
			return;
		}
		if (this->teamspeak.setClientSelfVariableAsString(connection, CLIENT_META_DATA, data.c_str()) != ERROR_ok)
		{
			this->teamspeak.logMessage("Failed to update metadata", LogLevel_ERROR, "MARS", 0);
		}

		if (flush == true)
		{
			if (this->teamspeak.flushClientSelfUpdates(connection, nullptr) != ERROR_ok)
			{
				this->teamspeak.logMessage("Failed to flush metadata update", LogLevel_ERROR, "MARS", 0);
			}
		}
	}

	void Plugin::clearMetaData()
	{
		uint64 connection = this->teamspeak.getCurrentServerConnectionHandlerID();
		if (!connection)
		{
			return;
		}

		if (this->teamspeak.setClientSelfVariableAsString(connection, CLIENT_META_DATA, "") != ERROR_ok)
		{
			this->teamspeak.logMessage("Failed to clear metadata", LogLevel_ERROR, "MARS", 0);
		}
		if (this->teamspeak.flushClientSelfUpdates(connection, nullptr) != ERROR_ok)
		{
			this->teamspeak.logMessage("Failed to flush metadata update", LogLevel_ERROR, "MARS", 0);
		}
	}

	void Plugin::start()
	{
		this->usingVAD = this->usingVoiceActivation();
		if (this->usingVAD)
		{
			this->disableVoiceActivation();
		}

		this->inGame = true;
		this->updateMetaData(true);
	}

	void Plugin::stop()
	{
		if (this->usingVAD)
		{
			this->enableVoiceActivation();
		}

		this->inGame = false;
		this->updateMetaData(true);
	}

	/// <summary>
	/// Returns the current voice activation detection setting
	/// </summary>
	/// <returns>True if using voice activation, otherwise false</returns>
	bool Plugin::usingVoiceActivation() const
	{
		uint64 connection = this->teamspeak.getCurrentServerConnectionHandlerID();
		bool vad = false;

		char* result;
		if (this->teamspeak.getPreProcessorConfigValue(connection, "vad", &result) == ERROR_ok)
		{
			if (strcmp(result, "true") == 0)
			{
				vad = true;
			}
			else
			{
				vad = false;
			}

			this->teamspeak.freeMemory(result);
		}
		else
		{
			// Failed to get value
			throw;
		}

		return vad;
	}

	/// <summary>
	/// Enables voice activation detection
	/// </summary>
	void Plugin::enableVoiceActivation() const
	{
		uint64 connection = this->teamspeak.getCurrentServerConnectionHandlerID();

		// Activate VAD
		if (this->teamspeak.setPreProcessorConfigValue(connection, "vad", "true") != ERROR_ok)
		{
		}

		// Activate mic input
		if (this->teamspeak.setClientSelfVariableAsInt(connection, CLIENT_INPUT_DEACTIVATED, INPUT_ACTIVE) != ERROR_ok)
		{
		}
	}

	/// <summary>
	/// Disables voice activation detection
	/// </summary>
	void Plugin::disableVoiceActivation() const
	{
		uint64 connection = this->teamspeak.getCurrentServerConnectionHandlerID();

		// Disable VAD
		if (this->teamspeak.setPreProcessorConfigValue(connection, "vad", "false") != ERROR_ok)
		{
		}

		// Deactivate mic input
		if (this->teamspeak.setClientSelfVariableAsInt(connection, CLIENT_INPUT_DEACTIVATED, INPUT_DEACTIVATED) != ERROR_ok)
		{
		}
	}

	/// <summary>
	/// Activates specified radio number (not index)
	/// </summary>
	void Plugin::selectActiveRadio(int id)
	{
		if (id < 0 || id > 3)
		{
			throw "Invalid radio";
		}

		if (id - 1 != this->selectedRadioIndex)
		{
			this->selectedRadioIndex = id - 1;
			this->updateMetaData(true);
		}
	}

	/// <summary>
	/// Activates the internal radios
	/// </summary>
	void Plugin::useInternalRadios()
	{
		if (this->usingExternal)
		{
			this->usingExternal = false;
			this->updateMetaData(true);
		}
	}

	/// <summary>
	/// Activates the external radios
	/// </summary>
	void Plugin::useExternalRadios()
	{
		if (!this->usingExternal)
		{
			this->usingExternal = true;
			this->updateMetaData(true);
		}
	}

	void Plugin::setRadioInformation(bool internal, const string& name, int radio, int primary, int secondary, Modulation modulation)
	{
		int index = radio - 1;

		if (internal == true)
		{
			this->internal[index].setName(name);
			this->internal[index].setPrimaryFrequency(primary);
			this->internal[index].setSecondaryFrequency(secondary);
			this->internal[index].setModulation(modulation);
		}
		else
		{
			this->external[index].setName(name);
			this->external[index].setPrimaryFrequency(primary);
			this->external[index].setSecondaryFrequency(secondary);
			this->external[index].setModulation(modulation);
		}

		this->updateMetaData(true);
	}

	void Plugin::setPlayerInformation(const string& name, const string& unit)
	{
		this->name = name;
		this->unit = unit;
		this->updateMetaData();
	}

	void Plugin::setPosition(float x, float y, float z)
	{
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;
		this->updateMetaData();
	}
}

/* Unique name identifying this plugin */
const char* ts3plugin_name()
{
	return plugin.NAME;
}

/* Plugin version */
const char* ts3plugin_version()
{
	return plugin.VERSION;
}

/* Plugin API version. Must be the same as the clients API major version, else the plugin fails to load. */
int ts3plugin_apiVersion()
{
	return plugin.API_VERSION;
}

/* Plugin author */
const char* ts3plugin_author()
{
	return plugin.AUTHOR;
}

/* Plugin description */
const char* ts3plugin_description()
{
	return plugin.DESCRIPTION;
}

/* Set TeamSpeak 3 callback functions */
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs)
{
	plugin.setTeamSpeakFunctions(funcs);
}

/*
* Custom code called right after loading the plugin. Returns 0 on success, 1 on failure.
* If the function returns 1 on failure, the plugin will be unloaded again.
*/
int ts3plugin_init()
{
	plugin.loadConfiguration();
	plugin.initListener();
	plugin.initInput();
	plugin.updateMetaData();

	return 0;
	/* 0 = success, 1 = failure, -2 = failure but client will not show a "failed to load" warning */
	/* -2 is a very special case and should only be used if a plugin displays a dialog (e.g. overlay) asking the user to disable
	* the plugin again, avoiding the show another dialog by the client telling the user the plugin failed to load.
	* For normal case, if a plugin really failed to load because of an error, the correct return value is 1. */
}

/* Custom code called right before the plugin is unloaded */
void ts3plugin_shutdown()
{
	plugin.shutdownListener();
	plugin.shutdownInput();
	plugin.stop();
	plugin.clearMetaData();
}

/*
* If the plugin wants to use error return codes, plugin commands, hotkeys or menu items, it needs to register a command ID. This function will be
* automatically called after the plugin was initialized. This function is optional. If you don't use these features, this function can be omitted.
* Note the passed pluginID parameter is no longer valid after calling this function, so you must copy it and store it in the plugin.
*/
void ts3plugin_registerPluginID(const char* id)
{
	plugin.setPluginId(id);
}

/* Plugin command keyword. Return NULL or "" if not used. */
const char* ts3plugin_commandKeyword()
{
	return plugin.COMMAND_KEYWORD;
}

/* Plugin processes console command. Return 0 if plugin handled the command, 1 if not handled. */
int ts3plugin_processCommand(uint64 serverConnectionHandlerID, const char* command)
{
	if (plugin.processCommand(serverConnectionHandlerID, command) == true)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/* Client changed current server connection handler */
void ts3plugin_currentServerConnectionChanged(uint64 serverConnectionHandlerID)
{
}

/* Static title shown in the left column in the info frame */
const char* ts3plugin_infoTitle()
{
	return plugin.NAME;
}

/*
* Dynamic content shown in the right column in the info frame. Memory for the data string needs to be allocated in this
* function. The client will call ts3plugin_freeMemory once done with the string to release the allocated memory again.
* Check the parameter "type" if you want to implement this feature only for specific item types. Set the parameter
* "data" to NULL to have the client ignore the info data.
*/
void ts3plugin_infoData(uint64 serverConnectionHandlerID, uint64 id, enum PluginItemType type, char** data)
{
	if (type == PLUGIN_CLIENT)
	{
		string info = plugin.getClientInfoData(serverConnectionHandlerID, id);
		size_t size = info.length() + 1;
		*data = new char[size];
		strcpy_s(*data, size, info.c_str());
	}
}

/* Required to release the memory for parameter "data" allocated in ts3plugin_infoData and ts3plugin_initMenus */
void ts3plugin_freeMemory(void* data)
{
	delete[] data;
}

void ts3plugin_onConnectStatusChangeEvent(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber)
{
	if (newStatus == STATUS_CONNECTED)
	{
		plugin.updateMetaData(true);
	}
}

/* Callback */
void ts3plugin_onUpdateClientEvent(uint64 serverConnectionHandlerID, anyID clientID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier)
{
	plugin.onClientUpdated(serverConnectionHandlerID, clientID, invokerID);
}