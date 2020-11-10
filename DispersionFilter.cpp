#include "cdmt.hpp"
#include "FrequencyGeometry.hpp"
#include "DispersionFilter.hpp"

FrequencyGeometry fgeom (1024, 1370.0f, 16.0f);
DispersionFilter<DataPol::TwoPol, ExecPolicy::CPU> df (fgeom);
