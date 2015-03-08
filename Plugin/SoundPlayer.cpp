#include "SoundPlayer.h"

namespace MARS
{
	SoundPlayer::SoundPlayer()
		: directSound(nullptr)
		, primaryBuffer(nullptr)
		, secondaryBuffer(nullptr)
	{
	}

	void SoundPlayer::Initialize()
	{
		HRESULT result = DS_OK;

		// Create DirectSound interface
		result = DirectSoundCreate8(nullptr, &this->directSound, nullptr);
		if (FAILED(result))
		{
			throw "DirectSoundCreate8 failed";
		}

		// Set cooperative level
		result = this->directSound->SetCooperativeLevel(GetDesktopWindow(), DSSCL_PRIORITY);
		if (FAILED(result))
		{
			throw "SetCooperativeLevel failed";
		}

		// Create and configure primary buffer
		DSBUFFERDESC bufferDescription = { 0 };
		bufferDescription.dwSize = sizeof(bufferDescription);
		bufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLPAN;
		bufferDescription.dwBufferBytes = 0;
		bufferDescription.dwReserved = 0;
		bufferDescription.lpwfxFormat = nullptr;
		bufferDescription.guid3DAlgorithm = GUID_NULL;

		result = this->directSound->CreateSoundBuffer(&bufferDescription, &this->primaryBuffer, nullptr);
		if (FAILED(result))
		{
			throw "CreateSoundBuffer failed";
		}

		WAVEFORMATEX wf = { 0 };
		wf.wFormatTag = WAVE_FORMAT_PCM;
		wf.nChannels = 1;
		wf.nSamplesPerSec = 44100;
		wf.nAvgBytesPerSec = 44100 * (1 * 16 / 8);
		wf.nBlockAlign = 1 * 16 / 8;
		wf.wBitsPerSample = 16;
		wf.cbSize = 0;

		result = this->primaryBuffer->SetFormat(&wf);
		if (FAILED(result))
		{
			throw "SetFormat failed";
		}
	}

	void SoundPlayer::Load(const char* file)
	{
		HRESULT result = DS_OK;

		WAVEFORMATEX wf = { 0 };
		wf.wFormatTag = WAVE_FORMAT_PCM;
		wf.nChannels = 1;
		wf.nSamplesPerSec = 44100;
		wf.nAvgBytesPerSec = 44100 * (1 * 16 / 8);
		wf.nBlockAlign = 1 * 16 / 8;
		wf.wBitsPerSample = 16;
		wf.cbSize = 0;

		DSBUFFERDESC bd = { 0 };
		bd.dwSize = sizeof(bd);
		bd.dwFlags = DSBCAPS_CTRLPAN;
		bd.dwBufferBytes = 6176;
		bd.dwReserved = 0;
		bd.lpwfxFormat = &wf;
		bd.guid3DAlgorithm = GUID_NULL;

		IDirectSoundBuffer* tmp = nullptr;

		result = this->directSound->CreateSoundBuffer(&bd, &tmp, nullptr);
		if (FAILED(result))
		{
			throw "CreateSoundBuffer failed";
		}

		result = tmp->QueryInterface(IID_IDirectSoundBuffer8, (void**)&this->secondaryBuffer);
		if (FAILED(result))
		{
			throw "QueryInterface failed";
		}

		tmp->Release();
		tmp = nullptr;

		unsigned char data[6176];
		for (int i = 0; i < 6176; ++i)
		{
			data[i] = rand();
		}

		//this->secondaryBuffer->Lock(0, )

			//Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	}

	void SoundPlayer::Play(const char* file)
	{
		this->secondaryBuffer->Play(0, 0, 0);
	}

	SoundPlayer::~SoundPlayer()
	{
		if (this->secondaryBuffer != nullptr)
		{
			this->secondaryBuffer->Release();
		}

		if (this->primaryBuffer != nullptr)
		{
			this->primaryBuffer->Release();
		}

		if (this->directSound != nullptr)
		{
			this->directSound->Release();
		}
	}
}
