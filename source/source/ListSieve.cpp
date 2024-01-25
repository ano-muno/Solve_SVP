#include "ListSieve.h"

#include "RandomTools.h"
#include "YamlTools.h"
#include "Util.h"

namespace ListSieve {

	EuclidPoint RandomGenerateErrorVector(const int& dim_size, const ldouble& radius) {
		EuclidPoint Err = RandomTools::GenerateVectorUniformly_InHyperUnitBall(dim_size);
		Err.times(radius);
		return Err;
	}

	ErrorVectors Sampling(const LatticeBasis& Basis, const ldouble& radius) {
		EuclidPoint e = RandomGenerateErrorVector(Basis.dim_size, radius);
		EuclidPoint p = Basis.mod(e);
		return ErrorVectors(e, p);
	}

	void ListReduce(EuclidPoint* EP, const LatticePointList& LPList, const ldouble& delta, const LatticeBasis& Basis) {
        bool NoMoreReduced = false;
        while (!NoMoreReduced) {
            NoMoreReduced = true;
            for (const auto& ListPoint : LPList.List) {
                while (delta * LatticeUtil::norm((*EP)) > LatticeUtil::norm((*EP) - ListPoint)) {
                    *EP = *EP - ListPoint;
                    NoMoreReduced = false;
                }
            }
        }
        return;
	}

    LatticePoint ListSieve(const LatticeBasis& Basis, ldouble mu, ldouble xi,ExperimentTools::ExperimentData& ED) {
        ldouble delta = 1; 
        LatticePointList LPList;
        lint indexSup = static_cast<lint>(200 + pow(2, 1.35 * Basis.dim_size));
        lint index = 0;
        while (index < indexSup) {
            index++;
            LatticePoint LP;
            if(!ED.flag_FindLatticeVector_from_RandomSampling){
                ErrorVectors pe = Sampling(Basis, xi * mu);
                EuclidPoint p_first = pe.p;
                ListReduce(&pe.p, LPList, delta, Basis);
                LP = EuclidPoint_to_LatticePoint((pe.p - pe.e));
            }
            else{
                LP = ED.FLRS.FindCandidateLatticeVector();
            }
            if (!LPList.find(LP)) {
                for (const auto& ListPoint : LPList.List) {
                    if (dist(ListPoint, LP) < mu) {
                        if(ED.flag_CountingReptitions) ED.CR.total_reptitions = index; //experiment_CR
                        if(ED.flag_VolumeApproximation) ED.VA.update_HP(LP,true); //experiment_VA
                        return ListPoint - LP;
                    }
                }
                LPList.add(LP);
                if(ED.flag_VolumeApproximation) ED.VA.update_HP(LP); //experiment_VA
            }
            else {
                if(ED.flag_CountingReptitions) ED.CR.collision_time++; //experiment_CR
            }
        }
        return LatticePoint({ -1 });
    }

    SVPUtil::SVPResult ListSieve_main(LatticeBasis& Basis, lint IdentificationNumber, std::string comment, ldouble mu, ExperimentTools::ExperimentData& ED) {
        if(ED.flag_VolumeApproximation) ED.VA.set_up(Basis); //experiment_VA

        SVPUtil::SVPResult result("ListSieve");
        result.Basis = Basis;
        result.dimension_size = Basis.dim_size;
        if (IdentificationNumber == -1) result.Set_IdentificationNumber(RandomTools::uniform_integer_sampling(0, 1000000000));
        else result.Set_IdentificationNumber(IdentificationNumber);
        YamlTools::OutputYaml_InputData(result);

        Basis.LLLreduction();
        Basis.GetShortestVector_frompyBKZ();
        mu = LatticeUtil::norm(Basis.shortestvector)*1.005;
        
        Util::show(Basis);
        auto ListSieveSTART = std::chrono::high_resolution_clock::now();
        LatticePoint LP = ListSieve(Basis, mu, 0.685,ED);
        auto ListSieveEND = std::chrono::high_resolution_clock::now();
        auto ListSieveExecutionTime = std::chrono::duration_cast<std::chrono::milliseconds>(ListSieveEND - ListSieveSTART).count();
        std::cout << "Shortest Vector : " << Util::to_string(LP) << std::endl;
        std::cout << "Coefficient Vector : " << Util::to_string(Basis.CaluculateCoefficients(LP)) << std::endl;
        
        result.ShortestVector = LP;
        result.ShortestVectorNorm = LatticeUtil::norm(LP);
        result.ShortestVectorCoefficient = Basis.CaluculateCoefficients(LP);
        result.ExecuteTime = ListSieveExecutionTime;
        result.comment = comment;

        YamlTools::OutputYaml_Result(result);

        if(ED.flag_CountingReptitions) ED.CR.output(Basis.dim_size);
        if(ED.flag_VolumeApproximation) ED.VA.output();
        
        return result;
    }
}