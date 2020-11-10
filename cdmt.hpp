#pragma once
#include <iostream>
#include <fstream>

// string
#include <string>
#include <cstring>

// math
#include <math.h>
#include <complex>

// datastructure
#include <vector>

// cuda utils
#include "cdmt_cuda.hpp"
#include "cdmt_thrust.hpp"


using string_t = std::string;
using lindex = unsigned long;
using uindex = unsigned int;
using Byte   = char;
using PtrByte = Byte *;

using vb = std::vector<Byte>;
using vf = std::vector<float>;


constexpr float PI   = 3.141592653589793;
constexpr float TAU  = 6.283185307179586;
constexpr float DMCONSTANT = 2.41E-10;
