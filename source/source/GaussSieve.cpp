#include "GaussSieve.h"

#include "YamlTools.h"
#include "RandomTools.h"

namespace GaussSieve{
    lint norm2(LatticePoint LP){
        lint norm2_ret = 0;
        for(auto e:LP.vL) norm2_ret+=(e*e);
        return norm2_ret;
    }

    LatticePoint Sample(LatticeBasis Basis,ldouble xi){
        EuclidPoint EP = RandomTools::GenerateVectorUniformly_InHyperUnitBall(Basis.dim_size);
        EP.times(-20*xi);
        return Basis.ApproxCVP(EP);
    }

    void GaussReduce(LatticePoint& LP,LatticePointList &LPList,CandidateLatticePoints &CLPs){
        //List Reduce
        bool NoMoreListReduced = false;
        while (!NoMoreListReduced) {
            NoMoreListReduced = true;
            for(int i = 0;i<LPList.List.size();i++){
                if(LPList.IsErased(i)) continue;
                LatticePoint ListPoint = LPList.List[i]; 
                while ((norm2(LP) >= norm2(ListPoint)) && (norm2(LP) > norm2((LP) - ListPoint))) {
                    LP = LP - ListPoint;
                    NoMoreListReduced = false;
                }
            }
        }
        //
        bool NoMoreGaussReduced = false;
        while(!NoMoreGaussReduced){
            NoMoreGaussReduced = true;
            for(int i = 0;i<LPList.List.size();i++){
                if(LPList.IsErased(i)) continue;
                LatticePoint ListPoint = LPList.List[i];
                if((norm2(ListPoint) > norm2(LP)) && (norm2((ListPoint - LP)) = norm2(ListPoint))){
                    LPList.erase(i);
                    CLPs.add((ListPoint - LP));
                    NoMoreGaussReduced = false;
                }
            }
        }
        return;
    }

    LatticePoint GaussSieve_main(const LatticeBasis& Basis,lint mu2,ExperimentTools::ExperimentData& ED){
        lint collision_time = 0;
        const lint MAX_collision_time = 3000;
        ldouble xi = sqrt(static_cast<ldouble>(mu2));

        LatticePointList LPList;
        CandidateLatticePoints CLPs;
        while(collision_time < MAX_collision_time){
            LatticePoint LP;
            if(CLPs.IsEmpty()){
                LP = Sample(Basis,xi);
            }
            else{
                LP = CLPs.pop();
            }
            GaussReduce(LP,LPList,CLPs);
            if(norm2(LP)==0) {
                collision_time++;
                continue;
            }
            else{
                LPList.add(LP);
            }
            if(norm2(LP)<=mu2){
                if(norm2(LP)<mu2) {
                    std::cout << Util::to_string(LP) << std::endl;
                    assert(false);
                }
                return LP;
            }
        }

        return LatticePoint({-1});
    }

    SVPUtil::SVPResult GaussSieve(LatticeBasis& Basis, lint IdentificationNumber, std::string comment, lint mu2,ExperimentTools::ExperimentData& ED){

        SVPUtil::SVPResult result("GaussSieve");
        result.Basis = Basis;
        result.dimension_size = Basis.dim_size;
        if (IdentificationNumber == -1) result.Set_IdentificationNumber(RandomTools::uniform_integer_sampling(0, 1000000000));
        else result.Set_IdentificationNumber(IdentificationNumber);
        YamlTools::OutputYaml_InputData(result);

        Basis.LLLreduction();
        Basis.GetShortestVector_frompyBKZ();
        mu2 = norm2(Basis.shortestvector);

        Util::show(Basis);
        auto ListSieveSTART = std::chrono::high_resolution_clock::now();
        LatticePoint LP = GaussSieve_main(Basis, mu2,ED);
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

        return result;
    }
}