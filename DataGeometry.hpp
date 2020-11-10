#pragma once
#include "cdmt.hpp"
#include "ExecutionPolicy.hpp"

struct DataGeometry {
	uindex        nread;
	// overlap here is a single edge overlap
	uindex        noverlap;
	uindex        nblock;

	DataGeometry (uindex, uindex);
};


enum class DataType {
	Real, Complex
};

enum class DataPol {
	OnePol, TwoPol
};

template<DataType dt, ExecPolicy ep>
class DataArrays   {};

template<ExecPolicy ep>
class DataArrays<DataType::Real, ep>  {
	using real_type       = typename ExPolType<ep>::real_type;
	using real_atype      = typename ExPolType<ep>::real_atype;
	using complex_type    = typename ExPolType<ep>::complex_type;
	using complex_atype   = typename ExPolType<ep>::complex_atype;
	public:
		real_atype   for_d;
		real_atype   in_rovl_h;
};


template<ExecPolicy ep>
class DataArrays<DataType::Complex, ep> {
	using real_type       = typename ExPolType<ep>::real_type;
	using real_atype      = typename ExPolType<ep>::real_atype;
	using complex_type    = typename ExPolType<ep>::complex_type;
	using complex_atype   = typename ExPolType<ep>::complex_atype;
	public:
		complex_atype   for_d;
		complex_atype   in_rovl_h;
};


