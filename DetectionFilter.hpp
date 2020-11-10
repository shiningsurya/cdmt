#pragma once
#include "cdmt.hpp"

template<DataType dt, DataPol dp, ExecPolicy ep>
class DetectionFilter {
	using real_type       = typename ExPolType<ep>::real_type;
	using real_atype      = typename ExPolType<ep>::real_atype;
	using complex_type    = typename ExPolType<ep>::complex_type;
	using complex_atype   = typename ExPolType<ep>::complex_atype;
};

template<>
class DetectionFilter<DataType::Complex, DataPol::TwoPol, ExecPolicy::GPU> {
	using real_type       = typename ExPolType<ExecPolicy::GPU>::real_type;
	using real_atype      = typename ExPolType<ExecPolicy::GPU>::real_atype;
	using complex_type    = typename ExPolType<ExecPolicy::GPU>::complex_type;
	using complex_atype   = typename ExPolType<ExecPolicy::GPU>::complex_atype;

	typedef thrust::tuple<complex_type,complex_type> tup;
	public:
		struct det_func : public thrust::unary_function<tup, real_type> {

			__host__ __device__
			real_type operator() (const tup& a) const {
				complex_type x = thrust::get<0>(a);
				complex_type y = thrust::get<1>(a);
				real_type ret = 0.0f;
				ret = (x.x*x.x) + (x.y*x.y) + (y.x*y.x) + (y.y*y.y);
				ret = sqrtf (ret);
				return ret;
			}

		};

		void operator() (complex_atype& in, real_atype& out) {
			struct det_func func;

			auto half  = in.begin() + (in.size()/2);

			auto first = thrust::make_zip_iterator (
					thrust::make_tuple (
						in.begin(), half
						)
					);
			auto last  = thrust::make_zip_iterator (
					thrust::make_tuple (
						half, in.end()
						)
					);

			thrust::transform (first, last, out.begin(), func);
		}
};

#if 0
template<ExecPolicy ep> 
class Digitize {
	using real_type       = typename ExPolType<ep>::real_type;
	using real_atype      = typename ExPolType<ep>::real_atype;
	using complex_type    = typename ExPolType<ep>::complex_type;
	using complex_atype   = typename ExPolType<ep>::complex_atype;
	public:
		struct mom_func {
			
		};

		struct dig_func {
			const real_type  mean;
			const real_type  std;
			dig_func (real_type mean_, real_type std_) : mean (mean_), std (std_) {}

			byte_type operator(real_type& x) const {
				real_type temp = x - mean;
				temp /= std;
				return (byte_type) temp;
			}
			
		};

		void operator() (real_atype& in, byte_atype& out) {
			struct mom_func mom;

			struct dig_func dig (..);
			thrust::transform (in.begin(), in.end(), out.begin(), dig);
		}
};
#endif
