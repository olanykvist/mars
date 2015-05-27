#include "InputListener.h"
#include <tchar.h>

namespace MARS
{
	InputListener::InputListener()
		: pInput(nullptr)
		, vDevices()
		, listening(false)
	{
	}

	InputListener::~InputListener()
	{
		this->Stop();
	}

	void InputListener::Initialize()
	{
		HRESULT result = DI_OK;

		result = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&this->pInput, nullptr);
		if (FAILED(result))
		{
			// Log & throw
			throw;
		}

		result = this->pInput->EnumDevices(DI8DEVCLASS_ALL, InputListener::OnEnumDevice, this, DIEDFL_ATTACHEDONLY);
		if (FAILED(result))
		{
			// Log & throw
			throw;
		}
	}

	void InputListener::Start()
	{
		this->listening = true;
		this->Initialize();
		this->listenThread = thread(&InputListener::Listen, this);
	}

	void InputListener::Stop()
	{
		this->listening = false;

		if (this->listenThread.joinable())
		{
			this->listenThread.join();
		}

		for (auto& device : this->vDevices)
		{
			device->Unacquire();
			SafeReleaseInterface(device);
		}

		this->vDevices.clear();

		SafeReleaseInterface(this->pInput);
	}

	BOOL InputListener::OnEnumDevice(LPCDIDEVICEINSTANCE pDeviceInstance, void* pData)
	{
		HRESULT result = DI_OK;
		InputListener* listener = reinterpret_cast<InputListener*>(pData);
		IDirectInputDevice8* pDevice = nullptr;

		result = listener->pInput->CreateDevice(pDeviceInstance->guidInstance, &pDevice, nullptr);
		if (SUCCEEDED(result))
		{
			if (pDeviceInstance->wUsage == 0x04) // Generic joystick
			{
				result = pDevice->SetDataFormat(&c_dfDIJoystick2);
				result = pDevice->SetCooperativeLevel(nullptr, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
				result = pDevice->Acquire();
				listener->vDevices.push_back(pDevice);
			}
			else if ((pDeviceInstance->dwDevType & 0xff) == DI8DEVTYPE_KEYBOARD) // Keyboard
			{
				result = pDevice->SetDataFormat(&c_dfDIKeyboard);
				result = pDevice->SetCooperativeLevel(nullptr, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
				result = pDevice->Acquire();
				listener->vDevices.push_back(pDevice);
			}
		}
		else
		{

		}

		return DIENUM_CONTINUE;
	}

	void InputListener::Listen()
	{
		vector<byte[128]> last = vector<byte[128]>(this->vDevices.size());
		DIJOYSTATE2 joystickState = { 0 };
		BYTE keyboardState[256] = { 0 };
		DIDEVICEINSTANCE instance = { 0 };
		instance.dwSize = sizeof(instance);

		for (size_t i = 0; i < this->vDevices.size(); i++)
		{
			this->vDevices[i]->Poll();
			this->vDevices[i]->GetDeviceInfo(&instance);

			if (instance.wUsage == 0x04) // Joystick
			{
				this->vDevices[i]->GetDeviceState(sizeof(joystickState), &joystickState);
				for (size_t j = 0; j < 128; j++)
				{
					last[i][j] = joystickState.rgbButtons[j];
				}
			}
			else if ((instance.dwDevType & 0xff) == DI8DEVTYPE_KEYBOARD) // Keyboard
			{
				this->vDevices[i]->GetDeviceState(sizeof(keyboardState), &keyboardState);
				for (size_t j = 0; j < 128; j++)
				{
					last[i][j] = keyboardState[j];
				}
			}
		}

		Sleep(1000 / 60);

		while (this->listening == true)
		{
			for (int i = 0; i < this->vDevices.size(); i++) // Each device
			{
				this->vDevices[i]->Poll();
				this->vDevices[i]->GetDeviceInfo(&instance);

				if (instance.wUsage == 0x04)
				{
					this->vDevices[i]->GetDeviceState(sizeof(joystickState), &joystickState);

					for (int j = 0; j < 128; j++)
					{
						if ((joystickState.rgbButtons[j] & 0x80) && !(last[i][j] & 0x80))
						{
							if (this->ButtonDown != nullptr)
							{
								ButtonDown(instance.tszProductName, j);
							}
						}
						else if (!(joystickState.rgbButtons[j] & 0x80) && (last[i][j] & 0x80))
						{
							if (this->ButtonUp != nullptr)
							{
								ButtonUp(instance.tszProductName, j);
							}
						}

						last[i][j] = joystickState.rgbButtons[j];
					}
				}
				else if ((instance.dwDevType & 0xff) == DI8DEVTYPE_KEYBOARD) // Keyboard
				{
					this->vDevices[i]->GetDeviceState(sizeof(keyboardState), &keyboardState);

					for (int j = 0; j < 128; j++)
					{
						if ((keyboardState[j] & 0x80) && !(last[i][j] & 0x80))
						{
							if (this->ButtonDown != nullptr)
							{
							 	ButtonDown(instance.tszProductName, j);
							}
						}
						else if (!(keyboardState[j] & 0x80) && (last[i][j] & 0x80))
						{
							if (this->ButtonUp != nullptr)
							{
								ButtonUp(instance.tszProductName, j);
							}
						}

						last[i][j] = keyboardState[j];
					}
				}
			}

			Sleep(1000 / 60);
		}
	}

	void SafeReleaseInterface(IUnknown* pInterface)
	{
		if (pInterface != nullptr)
		{
			pInterface->Release();
			pInterface = nullptr;
		}
	}
}
