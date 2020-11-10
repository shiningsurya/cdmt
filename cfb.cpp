#include "cdmt.hpp"

#include "ExecutionPolicy.hpp"
#include "FrequencyGeometry.hpp"
#include "DataGeometry.hpp"

#include "Header.hpp"

#include "DADAFileSource.hpp"
#include "FilterbankSink.hpp"

#include "FFTEngine.hpp"
#include "DispersionFilter.hpp"
#include "DetectionFilter.hpp"


int main () {
	header_t hh = {};
	DADAFileSource dfs ("../2020-10-04-18:12:20_0000000000000000.000000.dada");
	//DADAFileSource dfs ("../2019-03-01-09:46:53_0000000000000000.000000.dada");
	dfs.ReadHeader (hh);
	lindex nsamples = hh.nsamples; 
	lindex psamples = hh.nsamples / hh.ndims; 
	lindex bsamples = hh.nsamples / hh.dpfac;

	constexpr auto ep  = ExecPolicy::GPU;
	constexpr auto dt  = DataType::Complex;
	constexpr auto pol = DataPol::TwoPol;

	/**
		Complex TwoPol
		FB.NCHANS = 4096
	**/
	int fbnchans = 2048;
	lindex fsamples  = bsamples / fbnchans;

	std::cout << "nsamples = " << nsamples << std::endl;
	std::cout << "psamples = " << psamples << std::endl;
	std::cout << "bsamples = " << bsamples << std::endl;
	std::cout << "fsamples = " << fsamples << std::endl;

	// fft
	// forward voltage FFT
	FFTGeometry<1> fg_in;
	fg_in.rdims[0]    = bsamples;
	fg_in.batch       = 2; // TwoPol
	// ---
	fg_in.inembed[0]  = bsamples;
	fg_in.istride     = 1;
	fg_in.idist       = bsamples;
	// ---
	fg_in.oembed[0]   = bsamples;
	fg_in.ostride     = 1;
	fg_in.odist       = bsamples;
	
	// backward filterbanking FFT
	FFTGeometry<1> fg_ou;
	fg_ou.rdims[0]    = fsamples;
	fg_ou.batch       = 2 * fbnchans; // TwoPol * FBNCHANS
	// ---
	fg_ou.inembed[0]  = fsamples;
	fg_ou.istride     = 1;
	fg_ou.idist       = fsamples;
	// ---
	fg_ou.oembed[0]   = fsamples;
	fg_ou.ostride     = 1;
	fg_ou.odist       = fsamples;

	FFTEngine<1, dt, ep>  fft_in (fg_in);
	std::cout << "FFTEngine IN done" << std::endl;
	FFTEngine<1, dt, ep>  fft_ou (fg_ou);
	std::cout << "FFTEngine OUT done" << std::endl;

	// dedisp
	FrequencyGeometry fgeom ((uindex)bsamples, hh.cfreq, hh.bandwidth);
	std::cout << fgeom;
	
	DispersionFilter<pol, ep> df (fgeom);

	DetectionFilter<dt, pol, ep> det;

	std::cout << "Dispersion Filter done" << std::endl;
	// memory

	constexpr auto cpu = ExecPolicy::CPU;
	constexpr auto gpu = ExecPolicy::GPU;
	typename ExPolType<gpu>::real_type  dm {71.022f};

	// host side
	typename ExPolType<cpu>::byte_atype      vbi_h     (nsamples);
	// typename ExPolType<cpu>::complex_atype   vin_h     (psamples);
	std::vector<typename ExPolType<gpu>::complex_type> vin_h (psamples);
	typename ExPolType<cpu>::byte_atype      fbmag_h   (bsamples);
	typename ExPolType<cpu>::real_atype      fbout_h   (bsamples);
	std::cout << "Host allocation done" << std::endl;
	// dev side
	typename ExPolType<gpu>::complex_atype   vin_d    (psamples);
	typename ExPolType<gpu>::complex_atype   fb_d     (psamples);
	typename ExPolType<gpu>::real_atype      fbmag_d  (bsamples);
	std::cout << "Device allocation done" << std::endl;

	std::cout << "Starting stuff" << std::endl;
	// loop here
	auto ptr = thrust::raw_pointer_cast (vbi_h.data());
	dfs.ReadData (ptr, nsamples);
	// dirty for loop
	// Assume ABABAB
	for (lindex i = 0, p=bsamples, j=0; i < bsamples; i++,p++) {
		vin_h[i].x =  vbi_h[j++];
		vin_h[i].y =  vbi_h[j++];
		vin_h[p].x =  vbi_h[j++];
		vin_h[p].y =  vbi_h[j++];
	}
	std::cout << "Data read" << std::endl;

	thrust::copy (vin_h.begin(), vin_h.end(), vin_d.begin());
	std::cout << "H2D copy" << std::endl;

	fft_in.Forward (vin_d, vin_d);
	std::cout << "FFT forward" << std::endl;
	df.DM (dm, vin_d, vin_d);
	std::cout << "DM'd" << std::endl;
	fft_ou.Backward (vin_d, fb_d);
	std::cout << "FFT backward" << std::endl;

	det (fb_d, fbmag_d);
	std::cout << "Detecting" << std::endl;

	thrust::copy (fbmag_d.begin(), fbmag_d.end(), fbout_h.begin());
	std::cout << "D2H copy" << std::endl;

	std::cout << " First ten" << std::endl;
	for (unsigned ji = 0; ji < 10; ji++) {
		std::cout << fbout_h[ji] << " ";
	}
	std::cout << std::endl;

	FILE * oo = fopen("20201004.dat", "wb");
	auto fptr = thrust::raw_pointer_cast (fbout_h.data());
	fwrite (fptr, sizeof(float), fbout_h.size(), oo);
	fclose (oo);

	//FilterbankSink fbs("20201004.fil");
	//fbs.Header (hh);
	//fbs.WriteData (...);

	return 0;
}

