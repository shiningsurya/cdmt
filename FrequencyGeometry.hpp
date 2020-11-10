#pragma once
#include "cdmt.hpp"
#include "Header.hpp"

class FrequencyGeometry {
	public:
		float      center;      
		uindex     nchans;
		float      bandwidth;
		float      fch1;
		float      foff;

		//ctor
		FrequencyGeometry (const float&, const float&, const uindex&);
		FrequencyGeometry (const uindex&, const float&, const float&);
		FrequencyGeometry (header_t&);

		// operator
		float operator[] (uindex i);

};

std::ostream& operator<< (std::ostream&, const FrequencyGeometry&);
