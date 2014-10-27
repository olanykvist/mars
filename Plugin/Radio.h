#ifndef MARS_RADIO_H
#define MARS_RADIO_H

#include "Transmission.h"

namespace MARS
{
	class Radio
	{
	public:
		Radio();
		bool IsReceiving(const Transmission& transmission) const;
	};
};

#endif
