#include "LatticeUtil.h"

#include "RandomTools.h"

namespace LatticeUtil {
    ldouble inner_product(std::vector<ldouble> vl1,std::vector<ldouble> vl2){
        assert(vl1.size()==vl2.size());
        ldouble ret = 0;
        for(int i = 0;i<vl1.size();i++) ret += vl1[i]*vl2[i];
        return ret;
    }

    ldouble norm(std::vector<ldouble> vl){
        ldouble ret = 0;
        for(auto e:vl) ret+=e*e;
        return sqrt(ret);
    }

    LatticePoint EuclidPoint_to_LatticePoint(const EuclidPoint& EP) {
        LatticePoint vL(std::vector<lint>(EP.dim_size, 0));
        for (int i = 0; i < EP.dim_size; i++) vL.vL[i] = static_cast<lint>(boost::multiprecision::round(EP.vE[i]));
        return vL;
    }
    ldouble norm(const EuclidPoint& EP) {
        ldouble sum = 0;
        for (const auto& x : EP.vE) sum += (x * x);
        return sqrt(sum);
    }

    ldouble dist(const EuclidPoint EP1, const EuclidPoint EP2) {
        return norm(EP1 - EP2);
    }

    ldouble inner_product(const EuclidPoint& EP1,const EuclidPoint& EP2) {
        assert(EP1.dim_size == EP2.dim_size);
        ldouble ret = 0;
        for (int i = 0; i < EP1.dim_size; i++) ret += EP1.vE[i] * EP2.vE[i];
        return ret;
    }

    ldouble angle(const EuclidPoint& EP1, const EuclidPoint& EP2) {
        ldouble cos_theta = inner_product(EP1, EP2) / (norm(EP1) * norm(EP2));
        return ((ldouble(180.0) * boost::multiprecision::acos(cos_theta)) / ldouble(3.14159265358979));
    }

    LatticeBasis GenerateRandomLatticeBasis(const int dim_size) {
        std::vector<std::vector<lint>> basis(dim_size, std::vector<lint>(dim_size, 0));
        for (int i = 0; i < dim_size; i++) {
            for (int j = 0; j < dim_size; j++) {
                if (i == 0 && j == 0) basis[i][j] = RandomTools::uniform_integer_sampling((pow(lint(2), (10 * dim_size))), pow(lint(2), (10 * dim_size + 1)));
                else if (i == 0) basis[j][i] = RandomTools::uniform_integer_sampling(0, basis[0][0]);
                else if (i == j) basis[i][j] = 1;
            }
        }
        return LatticeBasis(basis);
    }
}