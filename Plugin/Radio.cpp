#include "Radio.h"

namespace MARS
{
	Radio::Radio()
		: primary(0)
		, secondary(0)
		, pan(0.0f)
		, volume(1.0f)
		, modulation(AM)
		, name()
		, isTransmitting(false)
	{
	}

	int Radio::getPrimaryFrequency() const
	{
		return this->primary;
	}

	void Radio::setPrimaryFrequency(int frequency)
	{
		this->primary = frequency;
	}

	int Radio::getSecondaryFrequency() const
	{
		return this->secondary;
	}

	void Radio::setSecondaryFrequency(int frequency)
	{
		this->secondary = frequency;
	}

	float Radio::getPan() const
	{
		return this->pan;
	}
	
	void Radio::setPan(float pan)
	{
		this->pan = pan;
	}

	float Radio::getVolume() const
	{
		return this->volume;
	}

	void Radio::setVolume(float volume)
	{
		this->volume = volume;
	}

	Modulation Radio::getModulation() const
	{
		return this->modulation;
	}

	void Radio::setModulation(Modulation modulation)
	{
		this->modulation = modulation;
	}

	const std::string& Radio::getName() const
	{
		return this->name;
	}

	void Radio::setName(const std::string& name)
	{
		this->name = std::string(name);
	}

	bool Radio::getIsTransmitting() const
	{
		return this->isTransmitting;
	}

	void Radio::setIsTransmitting(bool isTransmitting)
	{
		this->isTransmitting = isTransmitting;
	}

	bool Radio::canReceive(const Transmission& transmission) const
	{
		int frequency = transmission.getFrequency();
		Modulation modulation = transmission.getModulation();

		if (this->isTransmitting)
		{
			return false;
		}

		if (frequency == 0)
		{
			return false;
		}

		if (modulation == this->modulation && (frequency == this->primary || frequency == this->secondary))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	Radio::~Radio()
	{
	}
}