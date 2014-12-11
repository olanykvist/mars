#ifndef MARS_RADIO_H
#define MARS_RADIO_H

#include <string>
#include "Modulation.h"
#include "Transmission.h"

namespace MARS
{
	class Radio
	{
	public:
		Radio();
		~Radio();
		int getPrimaryFrequency() const;
		void setPrimaryFrequency(int frequency);
		int getSecondaryFrequency() const;
		void setSecondaryFrequency(int frequency);
		float getPan() const;
		void setPan(float pan);
		Modulation getModulation() const;
		void setModulation(Modulation modulation);
		const std::string& getName() const;
		void setName(const std::string& name);
		bool canReceive(const Transmission& transmission) const;

	private:
		int primary;
		int secondary;
		float pan;
		Modulation modulation;
		std::string name;
	};
};

#endif
