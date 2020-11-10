#pragma once
#include "cdmt.hpp"

enum class ExecPolicy { CPU, GPU, OMP, TBB };

template<ExecPolicy>
class ExPolType {
	public:
		typedef float                              real_type;
		typedef std::complex<float>                complex_type;
		typedef std::vector<float>                 real_atype;
		typedef std::vector<std::complex<float>>   complex_atype;
};

template<>
class ExPolType<ExecPolicy::CPU> {
	public:
		typedef Byte                                      byte_type;
		typedef float                                     real_type;
		typedef std::complex<float>                       complex_type;
		typedef std::vector<Byte>                         byte_atype;
		typedef thrust::host_vector<float>                real_atype;
		typedef thrust::host_vector<std::complex<float>>  complex_atype;
};

template<>
class ExPolType<ExecPolicy::GPU> {
	public:
		typedef cufftReal                            real_type;
		typedef cufftComplex                         complex_type;
		typedef thrust::device_vector<cufftReal>     real_atype;
		typedef thrust::device_vector<cufftComplex>  complex_atype;
};
