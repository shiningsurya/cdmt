#pragma once
#include "cdmt.hpp"
#include "ExecutionPolicy.hpp"
#include "DataGeometry.hpp"
#include "FrequencyGeometry.hpp"

template<int r>
struct FFTGeometry {
	//
	std::array<int, r> rdims;
	int                batch;
	std::array<int, r> inembed;
	std::array<int, r> oembed;
	int                istride, idist;
	int                ostride, odist;
};

template<int rank, DataType dt, ExecPolicy ep> 
class FFTEngine {};

template<int rank>
class FFTEngine<rank, DataType::Complex, ExecPolicy::GPU> {
	using real_type       = typename ExPolType<ExecPolicy::GPU>::real_type;
	using real_atype      = typename ExPolType<ExecPolicy::GPU>::real_atype;
	using complex_type    = typename ExPolType<ExecPolicy::GPU>::complex_type;
	using complex_atype   = typename ExPolType<ExecPolicy::GPU>::complex_atype;
	private:
		// PlanMany
		using fgeom_t = FFTGeometry<rank>;


		// handles
		cufftHandle    handle;

		// geometries
		fgeom_t&  fg;

	public:
		FFTEngine (fgeom_t& for_) :
		fg (for_)  {
			// forward plan
			checkCudaErrors (
				cufftPlanMany (
					&handle, rank, fg.rdims.data(),
					fg.inembed.data(), fg.istride, fg.idist, 
					fg.oembed.data(),  fg.ostride, fg.odist, 
					CUFFT_C2C, fg.batch
				)
			);
			
		}

		void Forward (complex_atype& in, complex_atype& out) {
			// get ptrs
			auto d_in = thrust::raw_pointer_cast (in.data());
			auto d_ou = thrust::raw_pointer_cast (out.data());

			checkCudaErrors (
				cufftExecC2C (
					handle, d_in, d_ou, CUFFT_FORWARD
				)
			);
		}
			
		void Backward (complex_atype& in, complex_atype& out) {
			// get ptrs
			auto d_in = thrust::raw_pointer_cast (in.data());
			auto d_ou = thrust::raw_pointer_cast (out.data());

			checkCudaErrors (
				cufftExecC2C (
					handle, d_in, d_ou, CUFFT_INVERSE
				)
			);
		}

		~FFTEngine () {
			cufftDestroy (handle);
		}
};

#if 0
template<int rank>
class FFTEngine<rank, DataType::Real, ExecPolicy::GPU> {
	using real_type       = typename ExPolType<ExecPolicy::GPU>::real_type;
	using real_atype      = typename ExPolType<ExecPolicy::GPU>::real_atype;
	using complex_type    = typename ExPolType<ExecPolicy::GPU>::complex_type;
	using complex_atype   = typename ExPolType<ExecPolicy::GPU>::complex_atype;
	private:
		// PlanMany
		using fgeom_t = FFTGeometry<rank>;


		// handles
		cufftHandle    handle_for;
		cufftHandle    handle_bac;

		// geometries
		fgeom_t&  fg_for;
		fgeom_t&  fg_bac;

	public:
		FFTEngine (fgeom_t& for_, fgeom_t& bac_) :
		fg_for (for_), fg_bac (bac_) {
			// forward plan
			checkCudaErrors (
				cufftPlanMany (
					&handle_for, rank, fg_for.rdims.data(),
					fg_for.inembed.data(), fg_for.istride, fg_for.idist, 
					fg_for.oembed.data(),  fg_for.ostride, fg_for.odist, 
					CUFFT_R2C, fg_for.batch
				)
			);
			// backward plan
			checkCudaErrors (
				cufftPlanMany (
					&handle_bac, rank, fg_bac.rdims.data(),
					fg_bac.inembed.data(), fg_bac.istride, fg_bac.idist, 
					fg_bac.oembed.data(),  fg_bac.ostride, fg_bac.odist, 
					CUFFT_C2R, fg_bac.batch
				)
			);
			
		}


		void Forward (real_atype& in, complex_atype& out) {
			// get ptrs
			auto d_in = thrust::raw_pointer_cast (in.data());
			auto d_ou = thrust::raw_pointer_cast (out.data());

			checkCudaErrors (
				cufftExecR2C (
					handle_for, d_in, d_ou
				)
			);
		}
			
		void Backward (complex_atype& in, real_atype& out) {
			// get ptrs
			auto d_in = thrust::raw_pointer_cast (in.data());
			auto d_ou = thrust::raw_pointer_cast (out.data());

			checkCudaErrors (
				cufftExecC2R (
					handle_bac, d_in, d_ou
				)
			);
		}

		~FFTEngine () {
			cufftDestroy (handle_for);
			cufftDestroy (handle_bac);
		}
};
#endif
