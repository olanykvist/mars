// Standard includes

// TeamSpeak SDK includes
#include "public_errors.h"
#include "public_errors_rare.h"
#include "public_definitions.h"
#include "public_rare_definitions.h"
#include "ts3_functions.h"
#include "Plugin.h"
#include "MARS.h"

/* Unique name identifying this plugin */
const char* ts3plugin_name()
{
	return MARS::PLUGIN_NAME.c_str();
}

/* Plugin version */
const char* ts3plugin_version()
{
	return MARS::PLUGIN_VERSION.c_str();
}

/* Plugin API version. Must be the same as the clients API major version, else the plugin fails to load. */
int ts3plugin_apiVersion()
{
	return MARS::PLUGIN_API_LEVEL;
}

/* Plugin author */
const char* ts3plugin_author()
{
	return MARS::PLUGIN_AUTHOR.c_str();
}

/* Plugin description */
const char* ts3plugin_description()
{
	return MARS::PLUGIN_DESCRIPTION.c_str();
}

/* Set TeamSpeak 3 callback functions */
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs)
{
	MARS::ts = funcs;
}

/*
* Custom code called right after loading the plugin. Returns 0 on success, 1 on failure.
* If the function returns 1 on failure, the plugin will be unloaded again.
*/
int ts3plugin_init()
{
	MARS::listener.Initialize();
	MARS::listener.onMessage = MARS::OnMessageReceived;
	MARS::listener.Start();
	return 0;
	/* 0 = success, 1 = failure, -2 = failure but client will not show a "failed to load" warning */
	/* -2 is a very special case and should only be used if a plugin displays a dialog (e.g. overlay) asking the user to disable
	* the plugin again, avoiding the show another dialog by the client telling the user the plugin failed to load.
	* For normal case, if a plugin really failed to load because of an error, the correct return value is 1. */
}

/* Custom code called right before the plugin is unloaded */
void ts3plugin_shutdown()
{
	MARS::listener.Stop();
	MARS::listener.Destroy();
}

/*
* If the plugin wants to use error return codes, plugin commands, hotkeys or menu items, it needs to register a command ID. This function will be
* automatically called after the plugin was initialized. This function is optional. If you don't use these features, this function can be omitted.
* Note the passed pluginID parameter is no longer valid after calling this function, so you must copy it and store it in the plugin.
*/
void ts3plugin_registerPluginID(const char* id)
{
	MARS::pluginId = String(id);
}

/* Plugin command keyword. Return NULL or "" if not used. */
const char* ts3plugin_commandKeyword()
{
	return MARS::PLUGIN_KEYWORD.c_str();
}

/* Plugin processes console command. Return 0 if plugin handled the command, 1 if not handled. */
int ts3plugin_processCommand(uint64 serverConnectionHandlerID, const char* command)
{
	if (strcmp(command, "test") == 0)
	{
		MARS::ts.printMessageToCurrentTab("Testkommando!");
		return 0;
	}

	return 1;
}

/* Client changed current server connection handler */
void ts3plugin_currentServerConnectionChanged(uint64 serverConnectionHandlerID)
{
}

/* Static title shown in the left column in the info frame */
const char* ts3plugin_infoTitle()
{
	return MARS::PLUGIN_NAME.c_str();
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
		*data = new char[255];
		sprintf_s(*data, 255, "Info here...");
	}
}

/* Required to release the memory for parameter "data" allocated in ts3plugin_infoData and ts3plugin_initMenus */
void ts3plugin_freeMemory(void* data)
{
	delete[] data;
}