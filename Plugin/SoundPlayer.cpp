#include "SoundPlayer.h"

namespace MARS
{
	SoundPlayer::SoundPlayer()
		: directSound(nullptr)
		, primaryBuffer(nullptr)
	{
	}

	void SoundPlayer::Initialize()
	{
		const GUID GUID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

		// Create DirectSound interface
		HRESULT result = DirectSoundCreate8(&DSDEVID_DefaultPlayback, &this->directSound, nullptr);
		if (FAILED(result))
		{
			throw "FAIL";
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
	}

	SoundPlayer::~SoundPlayer()
	{
	}
}
