#ifndef MARS_TRANSMISSION_H
#define MARS_TRANSMISSION_H

#include "Modulation.h"
#include "Vector3.h"

namespace MARS
{
	class Transmission
	{
	public:
		Transmission(int frequency = 0, Modulation modulation = Modulation::AM);
		int getFrequency() const;
		void setFrequency(int frequency);
		Modulation getModulation() const;
		void setModulation(Modulation modulation);

	private:
		int frequency;
		Modulation modulation;
		Vector3 origin;
	};
};

#endif
