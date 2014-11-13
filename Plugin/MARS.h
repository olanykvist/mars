#ifndef MARS_MARS_H
#define MARS_MARS_H

#include <string>
#include <ts3_functions.h>
#include "SocketListener.h"

typedef std::string String;

namespace MARS
{
	extern const String PLUGIN_NAME;
	extern const String PLUGIN_VERSION;
	extern const String PLUGIN_AUTHOR;
	extern const String PLUGIN_DESCRIPTION;
	extern const String PLUGIN_KEYWORD;
	extern const int PLUGIN_API_LEVEL;
	extern const unsigned short PORT;
	extern String pluginId;
	extern TS3Functions ts;
	extern SocketListener listener;

	// Callback
	extern void OnMessageReceived(const char* message);
};

#endif