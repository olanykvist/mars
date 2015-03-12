#ifndef MARS_PLUGIN_H
#define MARS_PLUGIN_H

#ifdef PLUGIN_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <map>
#include "Radio.h"
#include "SocketListener.h"
#include "InputListener.h"
#include "ClientMetaData.h"
#include "Configuration.h"
#include "SoundPlayer.h"

namespace MARS
{
	class Plugin
	{
	public:
		static const char* NAME;
		static const char* VERSION;
		static const char* AUTHOR;
		static const char* DESCRIPTION;
		static const char* COMMAND_KEYWORD;
		static const int   API_VERSION;

		Plugin();
		~Plugin();

		void setTeamSpeakFunctions(TS3Functions functions);
		void setPluginId(const char* id);
		bool processCommand(uint64 serverConnectionHandlerID, const char* command);
		std::string getClientInfoData(uint64 serverConnectionHandlerId, uint64 clientId) const;
		std::string getClientMetaData(uint64 serverConnectionHandlerId, uint64 clientId) const;

		void initListener();
		void shutdownListener();

		void initInput();
		void shutdownInput();

		void initSoundPlayer();
		void shutdownSoundPlayer();

		void loadConfiguration();
		
		void onClientUpdated(uint64 serverConnectionHandlerId, anyID clientId, anyID invokerId);
		void onClientTalkStatusChanged(uint64 serverConnectionHandlerId, int status, anyID clientId);
		void onPlaybackVoiceDataEvent(uint64 serverConnectionHandlerId, anyID clientId, short* samples, int sampleCount, int channels);
		void onPostProcessVoiceDataEvent(uint64 serverConnectionHandlerId, anyID clientId, short* samples, int sampleCount, int channels, const unsigned int* channelSpeakerArray, unsigned int* channelFillMask);

		static void processAudio(short* samples, int sampleCount, int channels);

		static void onMessageReceived(const char* message);
		static void onButtonDown(const wchar_t* device, int button);
		static void onButtonUp(const wchar_t* device, int button);
		void updateMetaData(bool flush = false);
		void clearMetaData();
		void start();
		void stop();
		bool usingVoiceActivation() const;
		void enableVoiceActivation() const;
		void disableVoiceActivation() const;
		void selectActiveRadio(int id);
		void useInternalRadios();
		void useExternalRadios();
		void setRadioInformation(bool internal, const std::string& name, int radio, int primary, int secondary, Modulation modulation);
		void setPlayerInformation(const std::string& name, const std::string& unit);
		void setPosition(float x, float y, float z);

	private:
		TS3Functions teamspeak;
		char* pluginId;
		bool usingVAD;
		bool inGame;
		bool usingExternal;
		int selectedRadioIndex;
		std::vector<Radio> internal;
		std::vector<Radio> external;
		Radio* currentRadio;
		SocketListener socketListener;
		InputListener inputListener;
		SoundPlayer player;
		std::string name;
		std::string unit;
		ClientMetaData metaData;
		Vector3 position;
		Configuration configuration;
		std::map<anyID, Radio*> receivers;
	};
};

// TeamSpeak SDK functions
extern "C"
{
	/* Required functions */
	DLL_EXPORT const char* ts3plugin_name();
	DLL_EXPORT const char* ts3plugin_version();
	DLL_EXPORT int ts3plugin_apiVersion();
	DLL_EXPORT const char* ts3plugin_author();
	DLL_EXPORT const char* ts3plugin_description();
	DLL_EXPORT void ts3plugin_setFunctionPointers(const struct TS3Functions funcs);
	DLL_EXPORT int ts3plugin_init();
	DLL_EXPORT void ts3plugin_shutdown();

	/* Optional functions */
	DLL_EXPORT int ts3plugin_offersConfigure();
	DLL_EXPORT void ts3plugin_configure(void* handle, void* qParentWidget);
	DLL_EXPORT void ts3plugin_registerPluginID(const char* id);
	DLL_EXPORT const char* ts3plugin_commandKeyword();
	DLL_EXPORT int ts3plugin_processCommand(uint64 serverConnectionHandlerID, const char* command);
	DLL_EXPORT void ts3plugin_currentServerConnectionChanged(uint64 serverConnectionHandlerID);
	DLL_EXPORT const char* ts3plugin_infoTitle();
	DLL_EXPORT void ts3plugin_infoData(uint64 serverConnectionHandlerID, uint64 id, enum PluginItemType type, char** data);
	DLL_EXPORT void ts3plugin_freeMemory(void* data);
	DLL_EXPORT int ts3plugin_requestAutoload();
	DLL_EXPORT void ts3plugin_initMenus(struct PluginMenuItem*** menuItems, char** menuIcon);
	DLL_EXPORT void ts3plugin_initHotkeys(struct PluginHotkey*** hotkeys);

	/* Clientlib */
	DLL_EXPORT void ts3plugin_onConnectStatusChangeEvent(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber);
	DLL_EXPORT void ts3plugin_onNewChannelEvent(uint64 serverConnectionHandlerID, uint64 channelID, uint64 channelParentID);
	DLL_EXPORT void ts3plugin_onNewChannelCreatedEvent(uint64 serverConnectionHandlerID, uint64 channelID, uint64 channelParentID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier);
	DLL_EXPORT void ts3plugin_onDelChannelEvent(uint64 serverConnectionHandlerID, uint64 channelID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier);
	DLL_EXPORT void ts3plugin_onChannelMoveEvent(uint64 serverConnectionHandlerID, uint64 channelID, uint64 newChannelParentID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier);
	DLL_EXPORT void ts3plugin_onUpdateChannelEvent(uint64 serverConnectionHandlerID, uint64 channelID);
	DLL_EXPORT void ts3plugin_onUpdateChannelEditedEvent(uint64 serverConnectionHandlerID, uint64 channelID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier);
	DLL_EXPORT void ts3plugin_onUpdateClientEvent(uint64 serverConnectionHandlerID, anyID clientID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier);
	DLL_EXPORT void ts3plugin_onClientMoveEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage);
	DLL_EXPORT void ts3plugin_onClientMoveSubscriptionEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility);
	DLL_EXPORT void ts3plugin_onClientMoveTimeoutEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* timeoutMessage);
	DLL_EXPORT void ts3plugin_onClientMoveMovedEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, anyID moverID, const char* moverName, const char* moverUniqueIdentifier, const char* moveMessage);
	DLL_EXPORT void ts3plugin_onClientKickFromChannelEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, anyID kickerID, const char* kickerName, const char* kickerUniqueIdentifier, const char* kickMessage);
	DLL_EXPORT void ts3plugin_onClientKickFromServerEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, anyID kickerID, const char* kickerName, const char* kickerUniqueIdentifier, const char* kickMessage);
	DLL_EXPORT void ts3plugin_onClientIDsEvent(uint64 serverConnectionHandlerID, const char* uniqueClientIdentifier, anyID clientID, const char* clientName);
	DLL_EXPORT void ts3plugin_onClientIDsFinishedEvent(uint64 serverConnectionHandlerID);
	DLL_EXPORT void ts3plugin_onServerEditedEvent(uint64 serverConnectionHandlerID, anyID editerID, const char* editerName, const char* editerUniqueIdentifier);
	DLL_EXPORT void ts3plugin_onServerUpdatedEvent(uint64 serverConnectionHandlerID);
	DLL_EXPORT int  ts3plugin_onServerErrorEvent(uint64 serverConnectionHandlerID, const char* errorMessage, unsigned int error, const char* returnCode, const char* extraMessage);
	DLL_EXPORT void ts3plugin_onServerStopEvent(uint64 serverConnectionHandlerID, const char* shutdownMessage);
	DLL_EXPORT int  ts3plugin_onTextMessageEvent(uint64 serverConnectionHandlerID, anyID targetMode, anyID toID, anyID fromID, const char* fromName, const char* fromUniqueIdentifier, const char* message, int ffIgnored);
	DLL_EXPORT void ts3plugin_onTalkStatusChangeEvent(uint64 serverConnectionHandlerID, int status, int isReceivedWhisper, anyID clientID);
	DLL_EXPORT void ts3plugin_onConnectionInfoEvent(uint64 serverConnectionHandlerID, anyID clientID);
	DLL_EXPORT void ts3plugin_onServerConnectionInfoEvent(uint64 serverConnectionHandlerID);
	DLL_EXPORT void ts3plugin_onChannelSubscribeEvent(uint64 serverConnectionHandlerID, uint64 channelID);
	DLL_EXPORT void ts3plugin_onChannelSubscribeFinishedEvent(uint64 serverConnectionHandlerID);
	DLL_EXPORT void ts3plugin_onChannelUnsubscribeEvent(uint64 serverConnectionHandlerID, uint64 channelID);
	DLL_EXPORT void ts3plugin_onChannelUnsubscribeFinishedEvent(uint64 serverConnectionHandlerID);
	DLL_EXPORT void ts3plugin_onChannelDescriptionUpdateEvent(uint64 serverConnectionHandlerID, uint64 channelID);
	DLL_EXPORT void ts3plugin_onChannelPasswordChangedEvent(uint64 serverConnectionHandlerID, uint64 channelID);
	DLL_EXPORT void ts3plugin_onPlaybackShutdownCompleteEvent(uint64 serverConnectionHandlerID);
	DLL_EXPORT void ts3plugin_onSoundDeviceListChangedEvent(const char* modeID, int playOrCap);
	DLL_EXPORT void ts3plugin_onEditPlaybackVoiceDataEvent(uint64 serverConnectionHandlerID, anyID clientID, short* samples, int sampleCount, int channels);
	DLL_EXPORT void ts3plugin_onEditPostProcessVoiceDataEvent(uint64 serverConnectionHandlerID, anyID clientID, short* samples, int sampleCount, int channels, const unsigned int* channelSpeakerArray, unsigned int* channelFillMask);
	DLL_EXPORT void ts3plugin_onEditMixedPlaybackVoiceDataEvent(uint64 serverConnectionHandlerID, short* samples, int sampleCount, int channels, const unsigned int* channelSpeakerArray, unsigned int* channelFillMask);
	DLL_EXPORT void ts3plugin_onEditCapturedVoiceDataEvent(uint64 serverConnectionHandlerID, short* samples, int sampleCount, int channels, int* edited);
	DLL_EXPORT void ts3plugin_onCustom3dRolloffCalculationClientEvent(uint64 serverConnectionHandlerID, anyID clientID, float distance, float* volume);
	DLL_EXPORT void ts3plugin_onCustom3dRolloffCalculationWaveEvent(uint64 serverConnectionHandlerID, uint64 waveHandle, float distance, float* volume);
	DLL_EXPORT void ts3plugin_onUserLoggingMessageEvent(const char* logMessage, int logLevel, const char* logChannel, uint64 logID, const char* logTime, const char* completeLogString);

	/* Clientlib rare */
	DLL_EXPORT void ts3plugin_onClientBanFromServerEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, anyID kickerID, const char* kickerName, const char* kickerUniqueIdentifier, uint64 time, const char* kickMessage);
	DLL_EXPORT int  ts3plugin_onClientPokeEvent(uint64 serverConnectionHandlerID, anyID fromClientID, const char* pokerName, const char* pokerUniqueIdentity, const char* message, int ffIgnored);
	DLL_EXPORT void ts3plugin_onClientSelfVariableUpdateEvent(uint64 serverConnectionHandlerID, int flag, const char* oldValue, const char* newValue);
	DLL_EXPORT void ts3plugin_onFileListEvent(uint64 serverConnectionHandlerID, uint64 channelID, const char* path, const char* name, uint64 size, uint64 datetime, int type, uint64 incompletesize, const char* returnCode);
	DLL_EXPORT void ts3plugin_onFileListFinishedEvent(uint64 serverConnectionHandlerID, uint64 channelID, const char* path);
	DLL_EXPORT void ts3plugin_onFileInfoEvent(uint64 serverConnectionHandlerID, uint64 channelID, const char* name, uint64 size, uint64 datetime);
	DLL_EXPORT void ts3plugin_onServerGroupListEvent(uint64 serverConnectionHandlerID, uint64 serverGroupID, const char* name, int type, int iconID, int saveDB);
	DLL_EXPORT void ts3plugin_onServerGroupListFinishedEvent(uint64 serverConnectionHandlerID);
	DLL_EXPORT void ts3plugin_onServerGroupByClientIDEvent(uint64 serverConnectionHandlerID, const char* name, uint64 serverGroupList, uint64 clientDatabaseID);
	DLL_EXPORT void ts3plugin_onServerGroupPermListEvent(uint64 serverConnectionHandlerID, uint64 serverGroupID, unsigned int permissionID, int permissionValue, int permissionNegated, int permissionSkip);
	DLL_EXPORT void ts3plugin_onServerGroupPermListFinishedEvent(uint64 serverConnectionHandlerID, uint64 serverGroupID);
	DLL_EXPORT void ts3plugin_onServerGroupClientListEvent(uint64 serverConnectionHandlerID, uint64 serverGroupID, uint64 clientDatabaseID, const char* clientNameIdentifier, const char* clientUniqueID);
	DLL_EXPORT void ts3plugin_onChannelGroupListEvent(uint64 serverConnectionHandlerID, uint64 channelGroupID, const char* name, int type, int iconID, int saveDB);
	DLL_EXPORT void ts3plugin_onChannelGroupListFinishedEvent(uint64 serverConnectionHandlerID);
	DLL_EXPORT void ts3plugin_onChannelGroupPermListEvent(uint64 serverConnectionHandlerID, uint64 channelGroupID, unsigned int permissionID, int permissionValue, int permissionNegated, int permissionSkip);
	DLL_EXPORT void ts3plugin_onChannelGroupPermListFinishedEvent(uint64 serverConnectionHandlerID, uint64 channelGroupID);
	DLL_EXPORT void ts3plugin_onChannelPermListEvent(uint64 serverConnectionHandlerID, uint64 channelID, unsigned int permissionID, int permissionValue, int permissionNegated, int permissionSkip);
	DLL_EXPORT void ts3plugin_onChannelPermListFinishedEvent(uint64 serverConnectionHandlerID, uint64 channelID);
	DLL_EXPORT void ts3plugin_onClientPermListEvent(uint64 serverConnectionHandlerID, uint64 clientDatabaseID, unsigned int permissionID, int permissionValue, int permissionNegated, int permissionSkip);
	DLL_EXPORT void ts3plugin_onClientPermListFinishedEvent(uint64 serverConnectionHandlerID, uint64 clientDatabaseID);
	DLL_EXPORT void ts3plugin_onChannelClientPermListEvent(uint64 serverConnectionHandlerID, uint64 channelID, uint64 clientDatabaseID, unsigned int permissionID, int permissionValue, int permissionNegated, int permissionSkip);
	DLL_EXPORT void ts3plugin_onChannelClientPermListFinishedEvent(uint64 serverConnectionHandlerID, uint64 channelID, uint64 clientDatabaseID);
	DLL_EXPORT void ts3plugin_onClientChannelGroupChangedEvent(uint64 serverConnectionHandlerID, uint64 channelGroupID, uint64 channelID, anyID clientID, anyID invokerClientID, const char* invokerName, const char* invokerUniqueIdentity);
	DLL_EXPORT int  ts3plugin_onServerPermissionErrorEvent(uint64 serverConnectionHandlerID, const char* errorMessage, unsigned int error, const char* returnCode, unsigned int failedPermissionID);
	DLL_EXPORT void ts3plugin_onPermissionListGroupEndIDEvent(uint64 serverConnectionHandlerID, unsigned int groupEndID);
	DLL_EXPORT void ts3plugin_onPermissionListEvent(uint64 serverConnectionHandlerID, unsigned int permissionID, const char* permissionName, const char* permissionDescription);
	DLL_EXPORT void ts3plugin_onPermissionListFinishedEvent(uint64 serverConnectionHandlerID);
	DLL_EXPORT void ts3plugin_onPermissionOverviewEvent(uint64 serverConnectionHandlerID, uint64 clientDatabaseID, uint64 channelID, int overviewType, uint64 overviewID1, uint64 overviewID2, unsigned int permissionID, int permissionValue, int permissionNegated, int permissionSkip);
	DLL_EXPORT void ts3plugin_onPermissionOverviewFinishedEvent(uint64 serverConnectionHandlerID);
	DLL_EXPORT void ts3plugin_onServerGroupClientAddedEvent(uint64 serverConnectionHandlerID, anyID clientID, const char* clientName, const char* clientUniqueIdentity, uint64 serverGroupID, anyID invokerClientID, const char* invokerName, const char* invokerUniqueIdentity);
	DLL_EXPORT void ts3plugin_onServerGroupClientDeletedEvent(uint64 serverConnectionHandlerID, anyID clientID, const char* clientName, const char* clientUniqueIdentity, uint64 serverGroupID, anyID invokerClientID, const char* invokerName, const char* invokerUniqueIdentity);
	DLL_EXPORT void ts3plugin_onClientNeededPermissionsEvent(uint64 serverConnectionHandlerID, unsigned int permissionID, int permissionValue);
	DLL_EXPORT void ts3plugin_onClientNeededPermissionsFinishedEvent(uint64 serverConnectionHandlerID);
	DLL_EXPORT void ts3plugin_onFileTransferStatusEvent(anyID transferID, unsigned int status, const char* statusMessage, uint64 remotefileSize, uint64 serverConnectionHandlerID);
	DLL_EXPORT void ts3plugin_onClientChatClosedEvent(uint64 serverConnectionHandlerID, anyID clientID, const char* clientUniqueIdentity);
	DLL_EXPORT void ts3plugin_onClientChatComposingEvent(uint64 serverConnectionHandlerID, anyID clientID, const char* clientUniqueIdentity);
	DLL_EXPORT void ts3plugin_onServerLogEvent(uint64 serverConnectionHandlerID, const char* logMsg);
	DLL_EXPORT void ts3plugin_onServerLogFinishedEvent(uint64 serverConnectionHandlerID, uint64 lastPos, uint64 fileSize);
	DLL_EXPORT void ts3plugin_onMessageListEvent(uint64 serverConnectionHandlerID, uint64 messageID, const char* fromClientUniqueIdentity, const char* subject, uint64 timestamp, int flagRead);
	DLL_EXPORT void ts3plugin_onMessageGetEvent(uint64 serverConnectionHandlerID, uint64 messageID, const char* fromClientUniqueIdentity, const char* subject, const char* message, uint64 timestamp);
	DLL_EXPORT void ts3plugin_onClientDBIDfromUIDEvent(uint64 serverConnectionHandlerID, const char* uniqueClientIdentifier, uint64 clientDatabaseID);
	DLL_EXPORT void ts3plugin_onClientNamefromUIDEvent(uint64 serverConnectionHandlerID, const char* uniqueClientIdentifier, uint64 clientDatabaseID, const char* clientNickName);
	DLL_EXPORT void ts3plugin_onClientNamefromDBIDEvent(uint64 serverConnectionHandlerID, const char* uniqueClientIdentifier, uint64 clientDatabaseID, const char* clientNickName);
	DLL_EXPORT void ts3plugin_onComplainListEvent(uint64 serverConnectionHandlerID, uint64 targetClientDatabaseID, const char* targetClientNickName, uint64 fromClientDatabaseID, const char* fromClientNickName, const char* complainReason, uint64 timestamp);
	DLL_EXPORT void ts3plugin_onBanListEvent(uint64 serverConnectionHandlerID, uint64 banid, const char* ip, const char* name, const char* uid, uint64 creationTime, uint64 durationTime, const char* invokerName, uint64 invokercldbid, const char* invokeruid, const char* reason, int numberOfEnforcements, const char* lastNickName);
	DLL_EXPORT void ts3plugin_onClientServerQueryLoginPasswordEvent(uint64 serverConnectionHandlerID, const char* loginPassword);
	DLL_EXPORT void ts3plugin_onPluginCommandEvent(uint64 serverConnectionHandlerID, const char* pluginName, const char* pluginCommand);
	DLL_EXPORT void ts3plugin_onIncomingClientQueryEvent(uint64 serverConnectionHandlerID, const char* commandText);
	DLL_EXPORT void ts3plugin_onServerTemporaryPasswordListEvent(uint64 serverConnectionHandlerID, const char* clientNickname, const char* uniqueClientIdentifier, const char* description, const char* password, uint64 timestampStart, uint64 timestampEnd, uint64 targetChannelID, const char* targetChannelPW);

	/* Client UI callbacks */
	DLL_EXPORT void ts3plugin_onAvatarUpdated(uint64 serverConnectionHandlerID, anyID clientID, const char* avatarPath);
	DLL_EXPORT void ts3plugin_onMenuItemEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID);
	DLL_EXPORT void ts3plugin_onHotkeyEvent(const char* keyword);
	DLL_EXPORT void ts3plugin_onHotkeyRecordedEvent(const char* keyword, const char* key);
	DLL_EXPORT void ts3plugin_onClientDisplayNameChanged(uint64 serverConnectionHandlerID, anyID clientID, const char* displayName, const char* uniqueClientIdentifier);
}

#endif
