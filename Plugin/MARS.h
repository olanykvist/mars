#ifndef MARS_MARS_H
#define MARS_MARS_H

#include <string>
#include <ts3_functions.h>

typedef std::string String;

namespace MARS
{
	extern const String PLUGIN_NAME;
	extern const String PLUGIN_VERSION;
	extern const String PLUGIN_AUTHOR;
	extern const String PLUGIN_DESCRIPTION;
	extern const String PLUGIN_KEYWORD;
	extern const int PLUGIN_API_LEVEL;
	extern String pluginId;
	extern TS3Functions ts;
};

#endif
