#ifndef MARS_TRANSMISSION_H
#define MARS_TRANSMISSION_H

#include "Modulation.h"

namespace MARS
{
	class Transmission
	{
	public:
		Transmission(int frequency = 0, Modulation modulation = Modulation::AM);

	private:
		int frequency;
		Modulation modulation;
	};
};

#endif
