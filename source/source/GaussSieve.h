#pragma once

#include "common.h"
#include "LatticeUtil.h"
#include "SVPUtil.h"
#include "ExperimentTools.h"

#include <queue>

namespace GaussSieve{
	using LatticeUtil::LatticePoint;
	using LatticeUtil::EuclidPoint;
	using LatticeUtil::CoefficientVector;
	using LatticeUtil::LatticeBasis;

	struct LatticePointList{
		std::vector<LatticePoint> List;
		std::queue<int> ERASE_index_que;
		LatticePoint ERASED = LatticePoint(1);

		LatticePointList(){}

		void add(LatticePoint LP){
			if(ERASE_index_que.empty()) List.push_back(LP);
			else{
				int ind = ERASE_index_que.front();
				List[ind] = LP;
				ERASE_index_que.pop();
			}
			return;
		} 

		void erase(int ind){
			List[ind] = ERASED;
			ERASE_index_que.push(ind);
		}

		bool IsErased(int ind){
			return (List[ind]==ERASED);
		}
	};

	struct CandidateLatticePoints{
		std::queue<LatticePoint> que;

		CandidateLatticePoints(){}

		LatticePoint pop(){
			assert(!que.empty());
			LatticePoint ret = que.front();
			que.pop();
			return ret;
		}

		bool IsEmpty(){
			return que.empty();
		}

		void add(LatticePoint LP){
			que.push(LP);
		}
	};

	lint norm2(LatticePoint LP);
	LatticePoint Sample(LatticeBasis Basis,ldouble xi);
	void GaussReduce(LatticePoint& LP,LatticePointList &LPList,CandidateLatticePoints &CLPs);
	LatticePoint GaussSieve_main(const LatticeBasis& Basis,lint mu2,ExperimentTools::ExperimentData& ED);
	SVPUtil::SVPResult GaussSieve(LatticeBasis& Basis, lint IdentificationNumber, std::string comment, lint mu2,ExperimentTools::ExperimentData& ED);
}