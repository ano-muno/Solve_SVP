#pragma once

#include <vector>
#include <cassert>
#include <cmath>
#include <string>
#include <filesystem>
#include <fstream>

#include <Eigen/Dense>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <yaml-cpp/yaml.h>


using lint = boost::multiprecision::cpp_int;
using ldouble = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<100>>;

namespace Util{
    std::string pkBKZfilePATH();
	std::string pkLLLfilePATH();
	std::string datafolderPATH();
	std::string tmpfolderPATH();
	std::string tmp_SVinfo_yamlPATH();
	std::string tmp_LLLinfo_yamlPATH();
	std::string EXfolderPATH(); 
}