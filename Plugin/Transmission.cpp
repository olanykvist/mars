#include "Transmission.h"

namespace MARS
{
	Transmission::Transmission(int frequency, Modulation modulation)
		: frequency(frequency)
		, modulation(modulation)
	{
	}

	int Transmission::getFrequency() const
	{
		return this->frequency;
	}

	void Transmission::setFrequency(int frequency)
	{
		this->frequency = frequency;
	}

	Modulation Transmission::getModulation() const
	{
		return this->modulation;
	}

	void Transmission::setModulation(Modulation modulation)
	{
		this->modulation = modulation;
	}
}
