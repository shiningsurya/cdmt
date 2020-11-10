#pragma once
#include "cdmt.hpp"
#include "Header.hpp"
#include "Timer.hpp"
#include "DataGeometry.hpp"
#include "FFTEngine.hpp"

// CUDA includes
#include "cdmt_cuda.hpp"

// following convention in 
// cdmt paper: Bassa etal

template<DataType dt, Datapol dp, ExecPolicy ep>
class ConvolveFilterbank   {
	using real_type       = typename ExPolType<ep>::real_type;
	using real_atype      = typename ExPolType<ep>::real_atype;
	using complex_type    = typename ExPolType<ep>::complex_type;
	using complex_atype   = typename ExPolType<ep>::complex_atype;
	private:
		// fft
		FFTEngine<1, dt, ep>     fengine;
		FFTGeometry<1>           fg_in;
		FFTGeometry<1>           fg_ou;

		// dedisp
		DispersionFilter<dp,ep>  dedisp;
		FrequencyGeometry        fgeom;


		const DataGeometry      geom;
		header_t                   head;

		// host arrays
		vb               in_read_h;
		lindex           size_read_h;
		// out is filterbank digitized
		vb               out_h;
		lindex           size_out_h;
		// read+overlap is floats
		lindex           size_rovl_h;

		void AllocHostArrays ();

		// overlap
		// - overlaps in host
		// - transfers host array to device
		// XXX This design for ToOverlap means there will 
		// be a lot of IO. The upside is that our arrays 
		// can be large
		void ToOverlap ();
		// opposite of overlap
		// - remove the overlapped from the filterbank
		// - copy it to host after transpose
		void DeOverlap ();

		// Timers
#if 0
		Timer          t_copy_for("HostToDeviceCopy");
		Timer          t_copy_bac("DeviceToHostCopy");
		Timer          t_tooverlap("ToOverlap");
		Timer          t_deoverlap("DeOverlap");
#endif

		// misc
		unsigned int   nforwards;
		unsigned int   nbackwards;

	public:
		// ctor reads the header, fixes the geometry of the problem
		// alloc's all the arrays (host and device)
		ConvolveFilterbank (const header_t&, const ConvolveFilterbankGeometry& );

		// The main steps
		// All the geometry is already initiazed in ctor
		// Only host arrays are interfaced.
		// The class internally transfers to device, launches kernels and copys back to host
		void Forward  (vb&);
		void Backward (vb&);
		void Backward (vb&, float);

		// dtor
		// Data arrays are handles by the class
		~ConvolveFilterbank () {
			// free device arrays
			checkCudaErrors (cudaFree (in_d));
			checkCudaErrors (cudaFree (chirp_d));
		}

};
