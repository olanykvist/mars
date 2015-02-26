#ifndef MARS_INPUTLISTENER_H
#define MARS_INPUTLISTENER_H

#define INITGUID
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <vector>
#include <thread>

using std::vector;
using std::thread;

#pragma comment(lib, "dinput8.lib")

namespace MARS
{
	class InputListener
	{
	public:
		typedef void(*OnButtonEvent)(const wchar_t*, int);
		InputListener();
		~InputListener();
		void Start();
		void Stop();
		OnButtonEvent ButtonDown;
		OnButtonEvent ButtonUp;
		static BOOL CALLBACK OnEnumDevice(LPCDIDEVICEINSTANCE pDeviceInstance, void* pData);

	private:
		IDirectInput8* pInput;
		vector<IDirectInputDevice8*> vDevices;
		thread listenThread;
		bool listening;
		void Initialize();
		void Listen();
	};

	void SafeReleaseInterface(IUnknown* pInterface);
};

#endif
