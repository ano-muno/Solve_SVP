#pragma once

#include "common.h"
#include "LatticeUtil.h"
#include "SVPUtil.h"
#include "ExperimentTools.h"

namespace ListSieve{
	using LatticeUtil::LatticePoint;
	using LatticeUtil::EuclidPoint;
	using LatticeUtil::CoefficientVector;
	using LatticeUtil::LatticeBasis;

	struct ErrorVectors {
		EuclidPoint e{0};
		EuclidPoint p{0};
		ErrorVectors(EuclidPoint _e, EuclidPoint _p) { e = _e; p = _p; }
	};

	struct LatticePointList {
        std::vector<LatticePoint> List;
        int List_size, dim_size;
        Eigen::Matrix<double, -1, -1> polytope_mat;
        Eigen::Matrix<double, -1, 1> polytope_v;

        LatticePointList() {
            List.reserve(1000000);
            List_size = 0; dim_size = -1;
        }

        bool find(const LatticePoint& LP) {
            for (auto& _LP : List) if (_LP == LP) return true;
            return false;
        }

        void add(const LatticePoint& LP) {
            if (List_size == 0) dim_size = LP.dim_size;
            this->List.push_back(LP);
            //update_HP(vL);
            List_size++;
        }
	};

	EuclidPoint RandomGenerateErrorVector(const int& dim_size,const ldouble& radius);
	ErrorVectors Sampling(const LatticeBasis& Basis,const ldouble& radius);
	void ListReduce(EuclidPoint* EP,const LatticePointList& LPList,const ldouble& delta,const LatticeBasis& Basis);
	LatticePoint ListSieve(const LatticeBasis& Basis, ldouble mu, ldouble xi,ExperimentTools::ExperimentData& ED);
	SVPUtil::SVPResult ListSieve_main(LatticeBasis& Basis, lint IdentificationNumber, std::string comment, ldouble mu,ExperimentTools::ExperimentData& ED);
}
