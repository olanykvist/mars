#include "InputListener.h"
#include <tchar.h>

namespace MARS
{
	InputListener::InputListener()
		: pInput(nullptr)
		, vDevices()
	{
	}

	InputListener::~InputListener()
	{
		for (auto& device : this->vDevices)
		{
			device->Unacquire();
			SafeReleaseInterface(device);
		}

		this->vDevices.clear();

		SafeReleaseInterface(this->pInput);
	}

	void InputListener::Initialize()
	{
		HRESULT result = DI_OK;

		result = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&this->pInput, nullptr);
		if (FAILED(result))
		{
			// Log & throw
		}

		result = this->pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, InputListener::OnEnumDevice, this, DIEDFL_ATTACHEDONLY);
		if (FAILED(result))
		{
			// Log & throw
		}
	}

	void InputListener::Start()
	{
		this->Listen();
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
			else
			{

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
		DIJOYSTATE2 state = { 0 };
		DIPROPSTRING name;
		name.diph.dwSize = sizeof(DIPROPSTRING);
		name.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		name.diph.dwObj = 0; // device property 
		name.diph.dwHow = DIPH_DEVICE;
		TCHAR message[1024];
		
		for (size_t i = 0; i < this->vDevices.size(); i++)
		{
			this->vDevices[i]->Poll();
			this->vDevices[i]->GetDeviceState(sizeof(state), &state);
			for (size_t j = 0; j < 128; j++)
			{
				last[i][j] = state.rgbButtons[j];
			}
		}

		Sleep(1000 / 60);

		while (true)
		{
			for (size_t i = 0; i < this->vDevices.size(); i++) // Each device
			{
				this->vDevices[i]->Poll();
				this->vDevices[i]->GetDeviceState(sizeof(state), &state);

				for (size_t j = 0; j < 128; j++)
				{
					if ((state.rgbButtons[j] & 0x80) && !(last[i][j] & 0x80))
					{
						this->vDevices[i]->GetProperty(DIPROP_PRODUCTNAME, &name.diph);
						_stprintf_s(message, 1024, L"DOWN: %d %s\n", j, name.wsz);
						OutputDebugString(message);
					}
					else if (!(state.rgbButtons[j] & 0x80) && (last[i][j] & 0x80))
					{
						this->vDevices[i]->GetProperty(DIPROP_PRODUCTNAME, &name.diph);
						_stprintf_s(message, 1024, L"UP: %d %s\n", j, name.wsz);
						OutputDebugString(message);
					}

					last[i][j] = state.rgbButtons[j];
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
