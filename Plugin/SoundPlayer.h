#ifndef MARS_SOUNDPLAYER_H
#define MARS_SOUNDPLAYER_H

#define INIT_GUID
#include <dsound.h>

#pragma comment(lib, "dsound.lib")

namespace MARS
{
	class SoundPlayer
	{
	public:
		SoundPlayer();
		~SoundPlayer();
		void Initialize();

	private:
		IDirectSound8* directSound;
		IDirectSoundBuffer* primaryBuffer;
	};
};

#endif
