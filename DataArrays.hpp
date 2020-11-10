#pragma once
#include "cdmt.hpp"

#include "DataGeometry.hpp"
#include <cuda.h>
#include <cufft.h>
#include <helper_functions.h>
#include <helper_cuda.h>

template<enum DataGeometry>
class DataArrays {
	typedef  NULL dtype;
};

template<>
class DataArrays<DataGeometry::RealOnePol> {
	typedef  cufftReal dtype;
	public:
		cufftReal*      for_d;
		cufftReal*      in_rovl_h;
		DataArrays () : for_d (nullptr) {}
		~DataArrays () {
			if (for_d) checkCudaErrors (cudaFree (for_d));
		}
};

template<>
class DataArrays<DataGeometry::RealTwoPol> {
	typedef  cufftReal dtype;
	public:
		cufftReal*      for_1_d;
		cufftReal*      for_2_d;
		cufftReal*      in_rovl_h;
		DataArrays () :    for_1_d (nullptr), for_2_d (nullptr) {}
		~DataArrays () {
			if (for_1_d) checkCudaErrors (cudaFree (for_1_d));
			if (for_2_d) checkCudaErrors (cudaFree (for_2_d));
		}
};

template<>
class DataArrays<DataGeometry::ComplexOnePol> {
	typedef  cufftComplex dtype;
	public:
		cufftComplex*      for_d;
		cufftComplex*      in_rovl_h;
		DataArrays () :    for_d (nullptr) {}
		~DataArrays () {
			if (for_d) checkCudaErrors (cudaFree (for_d));
		}
};

template<>
class DataArrays<DataGeometry::ComplexTwoPol> {
	typedef  cufftComplex dtype;
	public:
		cufftComplex*      for_1_d;
		cufftComplex*      for_2_d;
		cufftComplex*      in_rovl_h;
		DataArrays () :    for_1_d (nullptr), for_2_d (nullptr) {}
		~DataArrays () {
			if (for_1_d) checkCudaErrors (cudaFree (for_1_d));
			if (for_2_d) checkCudaErrors (cudaFree (for_2_d));
		}
};
