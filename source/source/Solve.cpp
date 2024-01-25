#include "common.h"
#include "Util.h"
#include "LatticeUtil.h"
#include "ListSieve.h"
#include "GaussSieve.h"
#include "ExperimentTools.h"

#include <yaml-cpp/yaml.h> 

void Experiment(std::string testname){
    int dim_min = 30;
    int dim_max = 30;
    int samples_for_dimension = 3;

    for (int dim_size = dim_min;dim_size<=dim_max;dim_size+=5){
        for(int _ = 0;_<samples_for_dimension;_++){
            ExperimentTools::ExperimentData ED(true,false,false);

            std::cout << "dim : " + std::to_string(dim_size) + " /Num : " + std::to_string(_+1) + "/" + std::to_string(samples_for_dimension) << std::endl;
            LatticeUtil::LatticeBasis Basis = LatticeUtil::GenerateRandomLatticeBasis(dim_size);
            std::string comment = testname + ": dim" + std::to_string(dim_size) + "No" + std::to_string(_+1);
            SVPUtil::SVPResult result = ListSieve::ListSieve_main(Basis, -1, comment, -1 ,ED);            
        }
    }

    return;
}

void LoadStorage(lint IdentificationNumber){
    std::string inputfilename = "/" + IdentificationNumber.str() + ".yaml";
    std::string inputfilePATH = Util::datafolderPATH() + inputfilename;
    YAML::Node InputData = YAML::LoadFile(inputfilePATH);
    int dim_size = InputData["InputData"]["dim_size"].as<int>();
    auto Basis_string = InputData["InputData"]["LatticeBasis"].as<std::vector<std::vector<std::string>>>();
    std::vector<std::vector<lint>> _basis(dim_size,std::vector<lint>(dim_size));
    for(int i = 0;i<dim_size;i++) for(int j = 0;j<dim_size;j++) _basis[i][j].assign(Basis_string[i][j]);
    LatticeUtil::LatticeBasis Basis = LatticeUtil::LatticeBasis(_basis);

    bool flag_CR = false;
    bool flag_VA = false;
    std::string input;
    std::cout << "CountingReptitions?(y/n)" << std::endl;
    std::cin >> input;
    if(input == "y") flag_CR = true;
    std::cout << "VolumeApproximation?(y/n)" << std::endl;
    std::cin >> input;
    if(input == "y") flag_VA = true;

    ExperimentTools::ExperimentData ED(flag_CR,flag_VA);

    std::cout << "If necessary , write comment." << std::endl;
    std::string comment; std::cin >> comment;

    std::cout << "Use GaussSieve instead of ListSieve ? (y/n)" << std::endl;
    std::cin >> input;
    SVPUtil::SVPResult result;
    if(input=="y"){
        std::cout << "Start GaussSieve" << std::endl;
        result = GaussSieve::GaussSieve(Basis, IdentificationNumber, comment, -1,ED);
    }else{
        std::cout << "Start ListSieve" << std::endl;
        result = ListSieve::ListSieve_main(Basis, IdentificationNumber, comment, -1 ,ED);
    }
    result.show();
    std::cout << "LLL-Basis coefficient : " << Util::to_string(Basis.CaluculateCoefficients(result.ShortestVector)) << std::endl;
}

void TrySample(){
    int dim_size;
    LatticeUtil::LatticeBasis Basis;
    ldouble mu = -1;
    lint IdentificationNumber = -1;

    std::cout << "Input Dimension Size." << std::endl;
    std::cin >> dim_size;

    std::string input;
    std::cout << "Generate Random Lattice Basis or Input Manually? (r/m)" << std::endl;
    std::cin >> input;
    if (input == "r") {
        Basis = LatticeUtil::GenerateRandomLatticeBasis(dim_size);
    }
    else if (input == "m") {
        std::cout << "Input Matrix." << std::endl;
        std::vector<std::vector<lint>> basisv(dim_size, std::vector<lint>(dim_size));
        for (int i = 0; i < dim_size; i++) for (int j = 0; j < dim_size; j++) std::cin >> basisv[i][j];
        Basis = LatticeUtil::LatticeBasis(basisv);
    }
    else {
        std::cout << "Invalid Input." << std::endl;
        return;
    }   

    bool flag_CR = false;
    bool flag_VA = false;
    std::cout << "CountingReptitions?(y/n)" << std::endl;
    std::cin >> input;
    if(input == "y") flag_CR = true;
    std::cout << "VolumeApproximation?(y/n)" << std::endl;
    std::cin >> input;
    if(input == "y") flag_VA = true;

    ExperimentTools::ExperimentData ED(flag_CR,flag_VA);

    std::cout << "If necessary , write comment." << std::endl;
    std::string comment; std::cin >> comment;

    std::cout << "Use GaussSieve instead of ListSieve ? (y/n)" << std::endl;
    std::cin >> input;
    SVPUtil::SVPResult result;
    if(input=="y"){
        std::cout << "Start GaussSieve" << std::endl;
        result = GaussSieve::GaussSieve(Basis, IdentificationNumber, comment, -1,ED);
    }else{
        std::cout << "Start ListSieve" << std::endl;
        result = ListSieve::ListSieve_main(Basis, IdentificationNumber, comment, mu ,ED);
    }
    result.show();
    std::cout << "LLL-Basis coefficient : " << Util::to_string(Basis.CaluculateCoefficients(result.ShortestVector)) << std::endl;
}

void tmpex(){
    std::cout << "Input dimensionsize" << std::endl;
    int dim_size;std::cin >> dim_size;
    std::cout << "Input Matrix." << std::endl;
    std::vector<std::vector<lint>> basisv(dim_size, std::vector<lint>(dim_size));
    for (int i = 0; i < dim_size; i++) for (int j = 0; j < dim_size; j++) std::cin >> basisv[i][j];
    LatticeUtil::LatticeBasis Basis = LatticeUtil::LatticeBasis(basisv);

    ExperimentTools::ExperimentData ED(false,false,false);
    SVPUtil::SVPResult result = GaussSieve::GaussSieve(Basis, -1, "tmpex", -1 ,ED);
}

int main() {
    std::string mode;
    std::cout << "Select Mode.(N/L/Ex)" << std::endl;
    std::cin >> mode;

    if(mode=="N"){
        TrySample();
    }
    else if(mode=="Ex"){
        std::string testname;
        std::cout << "Input Testname." << std::endl;
        std::cin >> testname;
        Experiment(testname);
    }
    else if(mode=="L"){
        lint IdentificationNumber;
        std::cout << "Input Identification Number" << std::endl;
        std::cin >> IdentificationNumber;
        LoadStorage(IdentificationNumber);
    }
    else{
        std::cout << "Invalid Input." << std::endl;
        tmpex();
    }
}