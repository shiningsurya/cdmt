#include "FrequencyGeometry.hpp"

FrequencyGeometry::FrequencyGeometry (header_t& h) {
	// recompute header
	h.Recompute ();

	center      =  h.cfreq;
	bandwidth   =  h.bandwidth;
	fch1        =  h.fch1;
	foff        =  h.foff;
	nchans      =  h.nchans;
}

FrequencyGeometry::FrequencyGeometry (
	const float& fch1_, 
	const float& foff_,
	const uindex& nch_) {

	fch1        =  fch1_;
	foff        =  foff_;
	nchans      =  nch_;

	bandwidth   =  foff * nchans;
	if (bandwidth < 0) bandwidth = -bandwidth;

	center      =  fch1 + (foff * nchans / 2);
}


FrequencyGeometry::FrequencyGeometry (
	const uindex& nch_,
	const float& cen_,
	const float& bw_) {

	nchans      =  nch_;
	bandwidth   =  bw_;
	if (bandwidth < 0) bandwidth = -bandwidth;
	center      =  cen_;

	fch1        =  center + (bandwidth/2);
	foff        =  -bandwidth / nchans;
}

float
FrequencyGeometry::operator[] (uindex i) {
	float ret;
	ret    = foff * i;
	ret   += fch1;
	return ret;
}

std::ostream&
operator<< (std::ostream& os, const FrequencyGeometry& fg) {
	os << "FCENTER      :    " << fg.center << std::endl;
	os << "BANDWIDTH    :    " << fg.bandwidth << std::endl;
	os << "FCH1         :    " << fg.fch1  << std::endl;
	os << "FOFF         :    " << fg.foff << std::endl;
	os << "NCHANS       :    " << fg.nchans << std::endl;
	return os;
}
