#include "Util.h"

namespace Util{

	std::string to_string(LatticeUtil::LatticePoint LP) {
		std::string ret = "[";
		for(auto e:LP.vL) ret = ret + e.str() + ",";
		ret.pop_back();
		ret = ret + "]";
		return ret;
	}

	std::string to_string(LatticeUtil::EuclidPoint EP) {
		std::string ret = "[";
		for(auto e:EP.vE) ret = ret + e.str(5) + ",";
		ret.pop_back();
		ret = ret + "]";
		return ret;
	}

	std::string to_string(LatticeUtil::CoefficientVector VC) {
		for(auto e:VC.vC) if(abs(e - boost::multiprecision::round(e))>0.1) {
			std::cout << "coef : " << to_string(LatticeUtil::EuclidPoint(VC.vC)) << std::endl;
			assert(false);
		}
		return to_string(LatticeUtil::EuclidPoint_to_LatticePoint(LatticeUtil::EuclidPoint(VC.vC)));
	}

	void show(LatticeUtil::LatticeBasis Basis) {
        std::cout << Basis.basis.transpose() << std::endl;
        std::cout << "Lattice Volume is : " << std::fixed << (Basis.volume()).str(10) << std::endl;
		std::cout << "Expected Shortest Vector Norm is : " << (sqrt((static_cast<ldouble>(Basis.dim_size)/(2*M_PI*M_E))) * powl(static_cast<long double>(Basis.volume()),1.0 / static_cast<long double>(Basis.dim_size))).str(10) << std::endl;
	}

	std::string pkBKZfilePATH() {
		YAML::Node pathinfo = YAML::LoadFile("../info/PATHDATA.yaml");
		return std::filesystem::absolute(pathinfo["PATHDATA"]["pyBKZfilePATH"].as<std::string>()).lexically_normal() ;
	}

	std::string pkLLLfilePATH() {
		YAML::Node pathinfo = YAML::LoadFile("../info/PATHDATA.yaml");
		return std::filesystem::absolute(pathinfo["PATHDATA"]["pyLLLfilePATH"].as<std::string>()).lexically_normal() ;
	}

	std::string datafolderPATH() {
		YAML::Node pathinfo = YAML::LoadFile("../info/PATHDATA.yaml");
		return  std::filesystem::absolute(pathinfo["PATHDATA"]["datafolderPATH"].as<std::string>()).lexically_normal();
	}

	std::string tmpfolderPATH() {
		YAML::Node pathinfo = YAML::LoadFile("../info/PATHDATA.yaml");
		return std::filesystem::absolute(pathinfo["PATHDATA"]["tmpfolderPATH"].as<std::string>()).lexically_normal();
	}

	std::string tmp_SVinfo_yamlPATH() {
		return tmpfolderPATH() + "/tmp_SVinfo.yaml";
	}

	std::string tmp_LLLinfo_yamlPATH() {
		return tmpfolderPATH() + "/tmp_LLLinfo.yaml";
	}

    std::string EXfolderPATH(){
        YAML::Node pathinfo = YAML::LoadFile("../info/PATHDATA.yaml");
        return std::filesystem::absolute(pathinfo["PATHDATA"]["exfolderPATH"].as<std::string>()).lexically_normal();
    }
}