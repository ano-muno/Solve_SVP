#pragma once

#include "common.h"
#include "LatticeUtil.h"

namespace RandomTools {
	lint uniform_integer_sampling(lint,lint);
	long double uniform_ldouble_sampling(long double, long double);
	LatticeUtil::EuclidPoint GenerateVectorUniformly_InHyperUnitBall(int dim_size);
}
