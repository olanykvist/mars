#ifndef MARS_SOUNDPLAYER_H
#define MARS_SOUNDPLAYER_H

#define INIT_GUID
#include <dsound.h>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")

namespace MARS
{
	class SoundPlayer
	{
	public:
		SoundPlayer();
		~SoundPlayer();
		void Initialize();
		void Play(float pan);
		void Load(const char* file);

	private:
		IDirectSound8* directSound;
		IDirectSoundBuffer* primaryBuffer;
		IDirectSoundBuffer8* secondaryBuffer;
	};
};

#endif
