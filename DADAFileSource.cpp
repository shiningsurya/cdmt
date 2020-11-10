#include "cdmt.hpp"
#include "DADAFileSource.hpp"

// for PRIu64
//#define __STDC_FORMAT_MACROS 1
#include <inttypes.h>

int DADAFileSource::ReadHeader (header_t& h) {
	int retval=0;

	// go to beginning
	fseek ( dd_fp, 0, SEEK_SET );

	// read from file
	fread ( ascii_header, sizeof(Byte), ASCII_HEADER_SIZE, dd_fp );

	int hsize=0;
	if (ascii_header_get (ascii_header, "HDR_SIZE", "%d", &hsize) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find header size" << std::endl;
		return -1;
	}
	if (hsize != ASCII_HEADER_SIZE) {
		std::cerr << "Found header size = " << hsize << std::endl;
	}
	
	// read ASCII header
	if (ascii_header_get (ascii_header, "FILE_SIZE", "%" PRIu64, &(h.nsamples)) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find nsamples" << std::endl;
		retval=-1;
	}

	if (ascii_header_get (ascii_header, "TSAMP", "%lf", &(h.tsamp)) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find tsamp" << std::endl;
		retval=-1;
	}
	h.tsamp /= 1E6; // stores as microseconds in DADA


	if (ascii_header_get (ascii_header, "FREQ", "%lf", &(h.cfreq)) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find freq" << std::endl;
		retval=-1;
	}
	if (ascii_header_get (ascii_header, "BW", "%lf", &(h.bandwidth)) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find bandwidth" << std::endl;
		retval=-1;
	}
	if (ascii_header_get (ascii_header, "NBIT", "%d", &(h.nbits)) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find bits" << std::endl;
		retval=-1;
	}
	if (ascii_header_get (ascii_header, "NPOL", "%d", &(h.npols)) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find npol" << std::endl;
		retval=-1;
	}
	if (ascii_header_get (ascii_header, "NCHAN", "%d", &(h.nchans)) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find nchans" << std::endl;
		retval=-1;
	}
	if (ascii_header_get (ascii_header, "NDIM", "%d", &(h.ndims)) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find ndim" << std::endl;
		retval=-1;
	}

	if (ascii_header_get (ascii_header, "SOURCE", "%s", h.name) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find source name" << std::endl;
		retval=-1;
	}
	if (ascii_header_get (ascii_header, "UTC_START", "%s", h.utc_start_str) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find source name" << std::endl;
		retval=-1;
	}
	if (ascii_header_get (ascii_header, "TELESCOPE", "%s", h.observatory) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find source name" << std::endl;
		retval=-1;
	}
	if (ascii_header_get (ascii_header, "MJD_START", "%lf", &(h.tstart)) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find RA" << std::endl;
		retval=-1;
	}

	if (ascii_header_get (ascii_header, "RA", "%s", h.ra) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find RA" << std::endl;
		retval=-1;
	}
	if (ascii_header_get (ascii_header, "DEC", "%s", h.dec) < 0) {
		std::cerr << "DADAFileSource::ReadHeader did not find DEC" << std::endl;
		retval=-1;
	}

	// data geometry
	if (h.ndims == 1)      type = DataType::Real;
	else if (h.ndims == 2) type = DataType::Complex;
	else {
		std::cerr << "DADAFileSource::ReadHeader NDIM not understood." << std::endl;
	}
	if (h.npols == 1)      pol  = DataPol::OnePol;
	else if (h.npols == 2) pol  = DataPol::TwoPol;
	else {
		std::cerr << "DADAFileSource::ReadHeader NPOL not understood." << std::endl;
	}

	h.dpfac = h.ndims * h.npols;
	return retval;
}

lindex DADAFileSource::ReadData (PtrByte dat, uindex len) {
	auto r = fread (dat, sizeof(Byte), len, dd_fp);
	return r;
}

