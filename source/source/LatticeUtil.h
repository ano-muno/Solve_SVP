#pragma once

#include "common.h"

namespace LatticeUtil{
    ldouble inner_product(std::vector<ldouble> vl1,std::vector<ldouble> vl2);
    ldouble norm(std::vector<ldouble> vl);

    struct LatticePoint {
        std::vector<lint> vL;
        int dim_size;

        LatticePoint(std::vector<lint> _vL = {0}) { vL = _vL; dim_size = vL.size(); }
        LatticePoint(int _dim_size) { vL = std::vector<lint>(_dim_size, 0); dim_size = _dim_size; }

        LatticePoint operator+(const LatticePoint& other) const {
            std::vector<lint> vLnow = this->vL;
            assert(vLnow.size() == other.vL.size());
            for (int i = 0; i < vLnow.size(); i++) vLnow[i] += other.vL[i];
            return vLnow;
        }

        LatticePoint operator-(const LatticePoint& other) const {
            std::vector<lint> vLnow = this->vL;
            assert(vLnow.size() == other.vL.size());
            for (int i = 0; i < vLnow.size(); i++) vLnow[i] -= other.vL[i];
            return vLnow;
        }

        bool operator==(const LatticePoint& other) const {
            for (int i = 0; i < this->dim_size; i++) {
                if (this->vL[i] != other.vL[i]) return false;
            }
            return true;
        }

        void times(const lint& k) {
            for (int i = 0; i < dim_size; i++) vL[i] *= k;
        }
    };

    struct EuclidPoint {
        std::vector<ldouble> vE;
        int dim_size;

        EuclidPoint(std::vector<ldouble> _vE) { vE = _vE; dim_size = vE.size(); }
        EuclidPoint(LatticePoint _LP) {
            dim_size = _LP.vL.size();
            vE.resize(dim_size);
            for (int i = 0; i < dim_size; i++) vE[i] = static_cast<ldouble>(_LP.vL[i]);
        }

        EuclidPoint operator+(const EuclidPoint& other) const {
            std::vector<ldouble> vEnow = this->vE;
            assert(vEnow.size() == other.vE.size());
            for (int i = 0; i < vEnow.size(); i++) vEnow[i] += other.vE[i];
            return vEnow;
        }

        EuclidPoint operator-(const EuclidPoint& other) const {
            std::vector<ldouble> vEnow = this->vE;
            assert(vEnow.size() == other.vE.size());
            for (int i = 0; i < vEnow.size(); i++) vEnow[i] -= other.vE[i];
            return vEnow;
        }

        void times(const ldouble& k) {
            for (int i = 0; i < dim_size; i++) vE[i] *= k;
        }
    };

    struct CoefficientVector {
        std::vector<ldouble> vC;
        int dim_size;
        CoefficientVector(std::vector<ldouble> _vC) { vC = _vC; dim_size = vC.size(); }
        CoefficientVector(int _dim_size) { dim_size = _dim_size; vC.resize(dim_size, 0); }

        void mod() {
            for (auto& e : this->vC) {
                e -= floor(e);
                if (e < 0) e += 1;
            }
        }
    };

    struct LatticeBasis {
        Eigen::Matrix<ldouble, Eigen::Dynamic, Eigen::Dynamic> basis;
        int dim_size;
        LatticePoint shortestvector{ 0 };
        bool Is_LLL_reduction = false;
        bool Calculate_GSOvec = false;

        LatticeBasis(const std::vector<std::vector<lint>>& _basis = { {-1} }) {
            dim_size = _basis[0].size();
            basis.resize(dim_size, _basis.size());
            for (int j = 0; j < _basis.size(); j++) for (int i = 0; i < dim_size; i++) basis(i, j) = static_cast<ldouble>(_basis[j][i]);
        }

        CoefficientVector CaluculateCoefficients(const EuclidPoint& EP) const {
            Eigen::Matrix<ldouble, Eigen::Dynamic, Eigen::Dynamic> EP_mat(EP.dim_size, 1);
            for (int i = 0; i < EP.dim_size; i++) EP_mat(i, 0) = static_cast<ldouble>(EP.vE[i]);
            std::vector<ldouble> v(EP.dim_size);
            Eigen::Matrix<ldouble, Eigen::Dynamic, Eigen::Dynamic> cm = basis.inverse() * EP_mat;
            for (int i = 0; i < EP.dim_size; i++) v[i] = cm(i, 0);
            CoefficientVector vC = CoefficientVector(v);
            return vC;
        }
        EuclidPoint CalculateEuclidianPoint(const CoefficientVector& vC) const {
            Eigen::Matrix<ldouble, Eigen::Dynamic, Eigen::Dynamic> vC_mat(vC.vC.size(), 1);
            for (int i = 0; i < vC.vC.size(); i++) vC_mat(i, 0) = static_cast<ldouble>(vC.vC[i]);
            Eigen::Matrix<ldouble, Eigen::Dynamic, Eigen::Dynamic> em = basis * vC_mat;
            std::vector<ldouble> vE(vC.vC.size());
            for (int i = 0; i < vC.vC.size(); i++) vE[i] = em(i, 0);
            return EuclidPoint(vE);
        }
        EuclidPoint mod(const EuclidPoint& EP) const {
            CoefficientVector vC = CaluculateCoefficients(EP);
            vC.mod();
            EuclidPoint vE_mod = CalculateEuclidianPoint(vC);
            return vE_mod;
        }

        ldouble volume() const {
            Eigen::Matrix<ldouble, Eigen::Dynamic, Eigen::Dynamic> tmp = basis * basis.transpose();
            return sqrt(tmp.determinant());
        }

        void OutputTmpyaml(std::string ReductionyamlfilePATH){
            YAML::Node InputData;

            InputData["InputData"]["dim_size"] = dim_size;
            std::vector<std::vector<std::string>> basisout(dim_size,std::vector<std::string>(dim_size));
            for(int i = 0;i<dim_size;i++) for(int j = 0;j<dim_size;j++) basisout[i][j] = basis(j,i).str();
            InputData["InputData"]["LatticeBasis"] = basisout;
            InputData["InputData"]["LatticeBasis"].SetStyle(YAML::EmitterStyle::Flow); 

            YAML::Emitter emit;
            emit << InputData;
            std::ofstream yamlfile(ReductionyamlfilePATH);
            yamlfile << emit.c_str();
            yamlfile.close();  

            return; 
        }

        void LLLreduction(){
            std::string ReductionyamlfilePATH = Util::tmpfolderPATH() + "/forpyreduction.yaml";
            OutputTmpyaml(ReductionyamlfilePATH);

            int ret = std::system(("python3 " + Util::pkLLLfilePATH() + " " + ReductionyamlfilePATH).c_str());
            assert(ret == 0);   
            YAML::Node LLLinfo = YAML::LoadFile(Util::tmp_LLLinfo_yamlPATH());
            auto Basis_string = LLLinfo["LLLBasis"].as<std::vector<std::vector<std::string>>>();
            std::vector<std::vector<lint>> _basis(dim_size,std::vector<lint>(dim_size));
            for(int i = 0;i<dim_size;i++) for(int j = 0;j<dim_size;j++) _basis[i][j].assign(Basis_string[i][j]);
            for (int j = 0; j < _basis.size(); j++) for (int i = 0; i < dim_size; i++) basis(i, j) = static_cast<ldouble>(_basis[j][i]);    
            
            Is_LLL_reduction = true;
            return;
        }

        void GetShortestVector_frompyBKZ(){
            std::string ReductionyamlfilePATH = Util::tmpfolderPATH() + "/forpyreduction.yaml";
            OutputTmpyaml(ReductionyamlfilePATH);

            int ret = std::system(("python3 " + Util::pkBKZfilePATH() + " " + ReductionyamlfilePATH).c_str());
            assert(ret == 0);
            YAML::Node SVinfo = YAML::LoadFile(Util::tmp_SVinfo_yamlPATH());
            std::vector<std::string> sv_string = SVinfo["shortestvector"].as<std::vector<std::string>>();
            std::vector<lint> sv(dim_size);
            for (int i = 0; i < dim_size; i++) sv[i].assign(sv_string[i]);
            shortestvector = sv;

            return;
        }

        std::vector<std::vector<ldouble>> GSOvec;
        void CalculateGSOvec(){
            GSOvec.resize(dim_size,std::vector<ldouble>(dim_size));
            for(int i = 0;i<dim_size;i++){
                std::vector<ldouble> bi(dim_size);
                for(int k = 0;k<dim_size;k++) bi[k] = basis(k,i);
                GSOvec[i] = bi;
                for(int j = 0;j<i;j++){
                    ldouble mu_ij = inner_product(GSOvec[j],bi) / (norm(GSOvec[j])*norm(GSOvec[j]));
                    for(int k = 0;k<dim_size;k++) GSOvec[i][k] -= mu_ij*GSOvec[j][k];
                }
            }
            Calculate_GSOvec = true;
            return;
        }

        //babai nearestplane 
        LatticePoint ApproxCVP(EuclidPoint EP) {
            if(!Calculate_GSOvec) CalculateGSOvec();
            std::vector<ldouble> b = EP.vE;
            for(int i = dim_size-1;i>=0;i--){
                ldouble c = boost::multiprecision::round(inner_product(GSOvec[i],b)/(norm(GSOvec[i])*norm(GSOvec[i])));
                for(int k=0;k<dim_size;k++) b[k] -= c*basis(k,i);
            }
            std::vector<lint> ret(dim_size);
            for(int k = 0;k<dim_size;k++) ret[k] = static_cast<lint>(boost::multiprecision::round(EP.vE[k]-b[k]));
            return LatticePoint(ret);
        }
    };

    LatticePoint EuclidPoint_to_LatticePoint(const EuclidPoint& EP);
	ldouble norm(const EuclidPoint& EP);
    ldouble dist(const EuclidPoint EP1, const EuclidPoint EP2);
	ldouble inner_product(const EuclidPoint& EP1,const EuclidPoint& EP2);
	ldouble angle(const EuclidPoint& EP1,const EuclidPoint& EP2);
    LatticeBasis GenerateRandomLatticeBasis(const int dim_size);
}