#include "YamlTools.h"

namespace YamlTools {
	void OutputYaml_InputData(SVPUtil::SVPResult Result) {
		if (!std::filesystem::exists(Result.yamlfolderPATH)){
			/* Folder Not Found Error*/
			assert(false);
		}
		if(std::filesystem::exists(Result.yamlfilePATH)) {
			/*Already File exist*/
			return;
		}
        YAML::Node InputData;

        InputData["IdentificationNumber"] = Result.IdentificationNumber.str();
        InputData["ResultNums"] = 0;
		InputData["InputData"]["dim_size"] = Result.dimension_size;
	    std::vector<std::vector<std::string>> basisout(Result.dimension_size,std::vector<std::string>(Result.dimension_size));
        for(int i = 0;i<Result.dimension_size;i++) for(int j = 0;j<Result.dimension_size;j++) basisout[i][j] = Result.Basis.basis(j,i).str();
        InputData["InputData"]["LatticeBasis"] = basisout;
        InputData["InputData"]["LatticeBasis"].SetStyle(YAML::EmitterStyle::Flow); 

        YAML::Emitter emit;
        emit << InputData;
        std::ofstream yamlfile(Result.yamlfilePATH);
        yamlfile << emit.c_str();
        yamlfile.close();

        std::cout << "saved : " << Result.yamlfilePATH << std::endl;
		/*Saved Correctly.*/
		return;
	}
	void OutputYaml_Result(SVPUtil::SVPResult Result) {
		if (!std::filesystem::exists(Result.yamlfolderPATH)){
			/* Folder Not Found Error*/
			assert(false);
		}
		if(!std::filesystem::exists(Result.yamlfilePATH)) {
			/*File Not Found*/
			return;
		}
        YAML::Node ResultData = YAML::LoadFile(Result.yamlfilePATH);
        int resultnums = ResultData["ResultNums"].as<int>();
        resultnums++;
        ResultData["ResultNums"] = resultnums;

        YAML::Node ThisResult;
        ThisResult["way"] = Result.way;
		ThisResult["Execute Time"] = Result.ExecuteTime.str();
        ThisResult["ShortestVector"] = Util::to_string(Result.ShortestVector);;
        ThisResult["ShortestVectorNorm"] =  Result.ShortestVectorNorm.str(5);
        ThisResult["ShortestVectorCoefficient"] = Util::to_string(LatticeUtil::EuclidPoint_to_LatticePoint(std::vector<ldouble>(Result.Basis.CaluculateCoefficients(Result.ShortestVector).vC)));
        ThisResult["Comment"] = Result.comment;

        ResultData["Result" + std::to_string(resultnums)].push_back(ThisResult);
        YAML::Emitter emit;
        emit << ResultData;
        std::ofstream yamlfile(Result.yamlfilePATH);
        yamlfile << emit.c_str();
        yamlfile.close();
		/*Saved Correctly.*/
		return;
	}
}