#include "SoundPlayer.h"
#include <fstream>

using std::ifstream;
using std::streampos;
using std::ios;

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
		DSBUFFERDESC bd = { 0 };
		bd.dwSize = sizeof bd;
		bd.dwFlags = DSBCAPS_PRIMARYBUFFER;
		bd.dwBufferBytes = 0;
		bd.dwReserved = 0;
		bd.lpwfxFormat = nullptr;
		bd.guid3DAlgorithm = GUID_NULL;

		result = this->directSound->CreateSoundBuffer(&bd, &(this->primaryBuffer), nullptr);
		if (FAILED(result))
		{
			throw "CreateSoundBuffer failed";
		}

		WAVEFORMATEX wf = { 0 };
		wf.wFormatTag = WAVE_FORMAT_PCM;
		wf.nChannels = 2;
		wf.wBitsPerSample = 16;
		wf.nSamplesPerSec = 44100;
		wf.nBlockAlign = wf.nChannels * (wf.wBitsPerSample / 8);
		wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
		wf.cbSize = 0;

		result = this->primaryBuffer->SetFormat(&wf);
		if (FAILED(result))
		{
			throw "SetFormat failed";
		}
	}

	void SoundPlayer::Load(const char* file)
	{
		streampos size;
		char* block;

		ifstream stream(file, ios::binary | ios::ate);
		if (stream.is_open())
		{
			size = stream.tellg();
			block = new char[size];
			stream.seekg(0, ios::beg);
			stream.read(block, size);
			stream.close();
		}

		HRESULT result = DS_OK;

		WAVEFORMATEX wf = { 0 };
		wf.wFormatTag = WAVE_FORMAT_PCM;
		wf.nChannels = 1;
		wf.wBitsPerSample = 16;
		wf.nSamplesPerSec = 44100;
		wf.nBlockAlign = wf.nChannels * (wf.wBitsPerSample / 8);
		wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
		wf.cbSize = 0;

		DSBUFFERDESC bd = { 0 };
		bd.dwSize = sizeof bd;
		bd.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPAN;
		bd.dwBufferBytes = size;
		bd.dwReserved = 0;
		bd.lpwfxFormat = &wf;
		bd.guid3DAlgorithm = GUID_NULL;

		IDirectSoundBuffer* tmp = nullptr;

		result = this->directSound->CreateSoundBuffer(&bd, &tmp, nullptr);
		if (FAILED(result))
		{
			throw "CreateSoundBuffer failed";
		}

		result = tmp->QueryInterface(IID_IDirectSoundBuffer8, (void**)&(this->secondaryBuffer));
		if (FAILED(result))
		{
			throw "QueryInterface failed";
		}

		tmp->Release();
		tmp = nullptr;

		unsigned char *bufferPtr;
		unsigned long bufferSize;
		result = this->secondaryBuffer->Lock(0, size, (void**)&bufferPtr, (DWORD*)&bufferSize, nullptr, 0, DSBLOCK_ENTIREBUFFER);
		if (FAILED(result))
		{
			throw "QueryInterface failed";
		}

		memcpy(bufferPtr, block, bufferSize);

		result = this->secondaryBuffer->Unlock((void*)bufferPtr, bufferSize, nullptr, 0);
		if (FAILED(result))
		{
			throw "QueryInterface failed";
		}

		delete[] block;
	}

	void SoundPlayer::Play(float pan)
	{
		long p = pan * 10000;
		this->secondaryBuffer->SetCurrentPosition(0);
		this->secondaryBuffer->SetPan(p);
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
