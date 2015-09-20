@echo off
set pluginpath=%LOCALAPPDATA%\TeamSpeak 3 Client\plugins
xcopy "..\bin\MARS.dll" "%pluginpath%" /y
