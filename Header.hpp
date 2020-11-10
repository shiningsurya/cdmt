#pragma once
#include "cdmt.hpp"

// One Header to rule them all

struct Header {
  // strings
  char utc_start_str[64];
  char name[16];
  char observatory[16];
  // positions
  char ra[16], dec[16];
  // frequency
  double cfreq, bandwidth;
  // time
  double tsamp, tstart;
  // memory
  int nbits, nchans, ndims, npols;
  lindex nsamples;

  // derived
  double tobs;
  double fch1,  foff;
  int dpfac;

	void Recompute ();
};

using header_t = struct Header;

std::ostream& operator<< (std::ostream& os, const header_t& );
