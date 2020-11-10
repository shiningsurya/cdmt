#include "cdmt.hpp"
#include "Header.hpp"



// cout
std::ostream& operator<< (std::ostream& os, const header_t& h) {
  // positions
  os << "RA     " << h.ra  << std::endl;
  os << "DEC    " << h.dec << std::endl;
  // frequency
  os << "CFREQ  " << h.cfreq << std::endl;
  os << "BANDW  " << h.bandwidth<< std::endl;
  // time
  os << "TSAMP  " << h.tsamp << std::endl;
  os << "TSTART " << h.tstart << std::endl;
  // memory
  os << "NSAMPS " << h.nsamples << std::endl;
  os << "NCHANS " << h.nchans<< std::endl;
  os << "NBITS  " << h.nbits << std::endl;
  os << "NPOL   " << h.npols<< std::endl;
  os << "NDIM   " << h.ndims << std::endl;
  // strings
  os << "UTC    " << h.utc_start_str << std::endl;
  os << "SOURCE " << h.name << std::endl;
  os << "OBS    " << h.observatory;
  return os;
}

void header_t::Recompute () {
	dpfac = ndims * npols;
	tobs  = nsamples / dpfac * tsamp;
	//
	foff  = -bandwidth / nchans;
	fch1  = cfreq + (bandwidth/2);
}
