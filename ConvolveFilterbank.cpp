#include "ConvolveFilterbank.hpp"


ConvolveFilterbankGeometry::ConvolveFilterbankGeometry (
	lindex nread_, lindex nchans_, lindex noverlap_
) : nread (nread_), nchans (nchans), noverlap(noverlap_) {
	naread   = nread + noverlap;
	nblock   = nread + 2*noverlap;
	nbackfft = nblock / nchans;
	nfb      = nread * nchans;
}


// ctors
// one dim, one pol
template<>
ConvolveFilterbank<DataGeometry::RealOnePol>::ConvolveFilterbank 
	(const header_t& h_, const ConvolveFilterbankGeometry& g_) :
	head (h_), geom (g_) {


	// sizes
	dpfac          = 1;
	size_read_h    = geom.naread * dpfac;
	size_rovl_h    = geom.nblock * dpfac;
	size_out_h     = geom.nfb * dpfac;
	size_ovl_h     = geom.noverlap * dpfac;
	
	// allocate host arrays
	AllocHostArrays ();
}

template<>
ConvolveFilterbank<DataGeometry::RealTwoPol>::ConvolveFilterbank 
	(const header_t& h_, const ConvolveFilterbankGeometry& g_) :
	head (h_), geom (g_) {

	// sizes
	dpfac          = 2;
	size_read_h    = geom.naread * dpfac;
	size_rovl_h    = geom.nblock * dpfac;
	size_out_h     = geom.nfb * dpfac;
	size_ovl_h     = geom.noverlap * dpfac;
	
	// allocate host arrays
	AllocHostArrays ();
}

template<>
ConvolveFilterbank<DataGeometry::ComplexOnePol>::ConvolveFilterbank 
	(const header_t& h_, const ConvolveFilterbankGeometry& g_) :
	head (h_), geom (g_) {

	// sizes
	dpfac          = 2;
	size_read_h    = geom.naread * dpfac;
	size_rovl_h    = geom.nblock * dpfac;
	size_out_h     = geom.nfb * dpfac;
	size_ovl_h     = geom.noverlap * dpfac;
	
	// allocate host arrays
	AllocHostArrays ();
}

template<>
ConvolveFilterbank<DataGeometry::ComplexTwoPol>::ConvolveFilterbank 
	(const header_t& h_, const ConvolveFilterbankGeometry& g_) :
	head (h_), geom (g_) {

	// sizes
	dpfac          = 4;
	size_read_h    = geom.naread * dpfac;
	size_rovl_h    = geom.nblock * dpfac;
	size_out_h     = geom.nfb * dpfac;
	size_ovl_h     = geom.noverlap * dpfac;
	
	// allocate host arrays
	AllocHostArrays ();
}

void ConvolveFilterbank::AllocHostArrays () {
	// read array
	in_read_h.resize (size_read_h, 0);

	// overlap array
	checkCudaErrors (cudaMallocHost (in_rovl_h, size_rovl_h*sizeof(dtype)));

	// out array
	checkCudaErrors (cudaMallocHost (out_h, size_rovl_h*sizeof(Byte)));

	// initialize counters
	nforwards  = 0;
	nbackwards = 0;
}

/***
	All overlap is same principle.
	copy last `noverlap` samples from end `in_rovl_h` to begin `in_rovl_h`.
	copy `naread` from `in_read_h` to `in_rovl_h[noverlap:]`
***/
template<>
void ConvolveFitlerbank<DataGeometry::ComplexTwoPol>::ToOverlap () {
	//
	dtype* j = in_rovl_h + size_rovl_h - 1;
	dtype* i = j  - size_ovl_h;
	thrust::copy (i, j, in_rovl_h);

	uindex m  = geom.noverlap;
	uindex n  = m + geom.naread;
	for (uindex k = 0; k < size_read_h; k+=4, m++, n++) {
		in_rovl_h[m].x = in_read_h[k];
		in_rovl_h[m].y = in_read_h[k+1];
		in_rovl_h[n].x = in_read_h[k+2];
		in_rovl_h[n].y = in_read_h[k+3];
	}

	if (  (m != geom.nblock) || (n != 2*geom.nblock) ) {
		std::cerr << "ConvolveFilterbank<ComplexTwoPol>::ToOverlap array copy end condition not reached" << std::endl;
	}
}

void ConvolveFitlerbank<DataGeometry dg>::ToOverlap () {}
#if 0
template<>
void ConvolveFitlerbank<RealTwoPol>::ToOverlap () {
	//
	cufftReal j = in_rovl_h + size_rovl_h - 1;
	cufftReal i = j  - size_ovl_h;
	std::copy (i, j, in_rovl_h);

	auto j = in_rovl_h + size_ovl_h;
	std::copy (in_read_h.begin(), in_read_h.end(), j);
	for (unsigned int k = 0, unsigned int l=size_ovl_h; k < geom.naread; k++, l++) {
		in_rovl_h[l] = in_read_h[k];
		in_rovl_h[l+geom.naread] = in_read_h[k];
	}
}
template<>
void ConvolveFitlerbank<RealOnePol>::ToOverlap () {
	//
	cufftReal j = in_rovl_h + size_rovl_h - 1;
	cufftReal i = j  - size_ovl_h;
	std::copy (i, j, in_rovl_h);

	auto j = in_rovl_h + size_ovl_h;
	std::copy (in_read_h.begin(), in_read_h.end(), j);

}
template<>
void ConvolveFitlerbank<RealTwoPol>::ToOverlap () {
	//
	cufftReal j = in_rovl_h + size_rovl_h - 1;
	cufftReal i = j  - size_ovl_h;
	std::copy (i, j, in_rovl_h);

	auto j = in_rovl_h + size_ovl_h;
	std::copy (in_read_h.begin(), in_read_h.end(), j);
	for (unsigned int k = 0, unsigned int l=size_ovl_h; k < geom.naread; k++, l++) {
		in_rovl_h[l] = in_read_h[k];
		in_rovl_h[l+geom.naread] = in_read_h[k];
	}
}
#endif
