#include "RandomTools.h"

#include <random>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

namespace RandomTools {
	lint uniform_integer_sampling(lint min, lint max) {
        boost::random::random_device seed_gen;
        boost::random::mt19937 gen(seed_gen());
        boost::random::uniform_int_distribution<lint> dist(min, max);
        return dist(gen);
	}

    long double uniform_ldouble_sampling(long double min, long double max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    // Gibbs Sampling TODO::Citation
    LatticeUtil::EuclidPoint GenerateVectorUniformly_InHyperUnitBall(int dim_size) {
        std::vector<std::vector<ldouble>> xs;
        std::vector<ldouble> x(dim_size, 0.0);

        for (int i = 0; i < 200 + 100; ++i) {
            ldouble dist = LatticeUtil::norm(x);
            for (int k = 0; k < dim_size; ++k) {
                dist -= x[k] * x[k];
                x[k] = static_cast<ldouble>(uniform_ldouble_sampling(-static_cast<long double>(boost::multiprecision::sqrt(1.0 - dist)), static_cast<long double>(boost::multiprecision::sqrt(1.0 - dist))));
                dist += x[k] * x[k];
            }
            if (i >= 100) xs.push_back(x);
        }
        return LatticeUtil::EuclidPoint(xs[static_cast<int>(uniform_integer_sampling(0, 199))]);
    }
}