#pragma once

#include "common.h"
#include "LatticeUtil.h"
#include "Util.h"

namespace SVPUtil {
    struct SVPResult {
        lint IdentificationNumber = -1;
        int dimension_size;
        LatticeUtil::LatticeBasis Basis;
        LatticeUtil::LatticePoint ShortestVector{0};
        ldouble ShortestVectorNorm;
        LatticeUtil::CoefficientVector ShortestVectorCoefficient{0};
        lint ExecuteTime;
        std::string way;
        std::string comment;
        std::string yamlfolderPATH;
        std::string yamlfilePATH;

        SVPResult(std::string _way = "ListSieve") {
            way = _way;
        };

        void Set_IdentificationNumber(lint _id) {
            if (IdentificationNumber != -1) {
                /*Error Occured*/
                return;
            }
            IdentificationNumber = _id;
            yamlfolderPATH = Util::datafolderPATH();
            yamlfilePATH = Util::datafolderPATH() + "/" + IdentificationNumber.str() + ".yaml";
        }

        void show(){
            std::cout << "SV : " + Util::to_string(ShortestVector) << std::endl;
            std::cout << "SV coefficient : " + Util::to_string(LatticeUtil::EuclidPoint_to_LatticePoint(std::vector<ldouble>(Basis.CaluculateCoefficients(ShortestVector).vC))) << std::endl;
            std::cout << "SV Norm: " + ShortestVectorNorm.str(10) << std::endl;
	    }
    };
}