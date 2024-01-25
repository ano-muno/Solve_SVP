#pragma once

#include "common.h"
#include "LatticeUtil.h"

namespace Util {
	std::string to_string(LatticeUtil::LatticePoint LP);
	std::string to_string(LatticeUtil::EuclidPoint EP);
	std::string to_string(LatticeUtil::CoefficientVector VC);
	void show(LatticeUtil::LatticeBasis basis);
}