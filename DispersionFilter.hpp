#pragma once
#include "cdmt.hpp"
#include "ExecutionPolicy.hpp"
#include "DataGeometry.hpp"
#include "FrequencyGeometry.hpp"

typedef thrust::tuple<uindex, cufftComplex> tup;
struct dd_func : public thrust::unary_function<tup, cufftComplex> {
	const FrequencyGeometry  fgeom;
	const float              tpow;
	const float              dmfac;

	/**
		For C2C the positive frequencies are first and then negative frequencies
		Complex samples, gT

	**/

	dd_func (const float dm, const FrequencyGeometry& fgeom_, float tpow_ = 80)
		: fgeom(fgeom_), tpow (tpow_), dmfac(dm * TAU / DMCONSTANT) {};

	__host__ __device__ cufftComplex operator() (const tup& a) const {
		// to account for multiple pol batch mode
		auto idx = thrust::get<0>(a);
		if (idx >= fgeom.nchans) idx -= fgeom.nchans;

		// half index
		auto hdx   = idx - fgeom.nchans/2;

		// get frequency
		// foff being negative is considered
		float freq = fgeom.center;
		float f    = hdx * fgeom.foff;

		// get taper
		//float t    = 1.0 / sqrtf(1.0 + powf (f/(0.47*fgeom.foff), tpow));
		float t    = 1.0;

		// compute phase
		float phase = - f * f * dmfac;
		phase          /= freq * freq * (freq + f);

		// actual real part and imaginary part
		auto    i = thrust::get<1>(a);
		float tc = t * cosf (phase);
		float ts = t * sinf (phase);

		// complex multiply and return
		cufftComplex ret;
		ret.x     = (tc * i.x) - (ts * i.y);
		ret.y     = (tc * i.y) + (ts * i.x);
		return ret;
	}
};

template<DataPol dp, ExecPolicy ep>
class DispersionFilter  {
	using real_type       = typename ExPolType<ep>::real_type;
	using real_atype      = typename ExPolType<ep>::real_atype;
	using complex_type    = typename ExPolType<ep>::complex_type;
	using complex_atype   = typename ExPolType<ep>::complex_atype;
	private:
	
		// functor


	public:
		const FrequencyGeometry&   fgeom;
		real_type                  taper_pow;

		// tors
		DispersionFilter (const FrequencyGeometry& fg, real_type tpow = 80) : fgeom(fg), taper_pow (tpow) {}
		~DispersionFilter () = default;

		// DM
		void DM (real_type dm, complex_atype& in, complex_atype& out) {
			// creator functor
			struct dd_func func (dm, fgeom, taper_pow);

			// thrust call
			size_t zero = 0;
			auto first = thrust::make_zip_iterator (
					thrust::make_tuple (
						thrust::make_counting_iterator(zero), in.begin()
						)
					);
			auto last  = thrust::make_zip_iterator (
					thrust::make_tuple (
						thrust::make_counting_iterator(in.size()), in.end()
						)
					);
			thrust::transform (first, last, out.begin(), func);
		}
};


#include "DispersionFilter.tpp"
