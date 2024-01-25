#pragma once

#include "common.h"
#include "Util.h"
#include "LatticeUtil.h"

#include <fstream>
#include <filesystem>

#include <volesti/cartesian_geom/cartesian_kernel.h>
#include <volesti/convex_bodies/hpolytope.h>
#include <volesti/generators/known_polytope_generators.h>
#include <volesti/random_walks/random_walks.hpp>
#include <volesti/volume/volume_cooling_balls.hpp>

namespace ExperimentTools{

    struct CountingReptitions{
        lint total_reptitions = 0;
        lint collision_time = 0;

        void output(int dim_size){
            std::string folderPATH = Util::EXfolderPATH() + "/CountingReptitions";
            std::string filePATH = folderPATH + "/ExData" + std::to_string(0) + ".yaml";

            YAML::Node Exyaml;
            if(std::filesystem::exists(filePATH)) Exyaml = YAML::LoadFile(filePATH);
            YAML::Node ThisData;
            ThisData["dimension size"] = dim_size; 
            ThisData["collision Times"] = collision_time.str();
            ThisData["Total Reptitions"] = total_reptitions.str();
            Exyaml.push_back(ThisData);

            YAML::Emitter emit;
            emit << Exyaml;
            std::ofstream yamlfile(filePATH);
            yamlfile << emit.c_str();
            yamlfile.close();
        }
    };

    struct VolumeApproximation{
        std::vector<int> List_size_vec;
        std::vector<double> Volume_ratio_vec;
        int List_size = 0;
        int dim_size = 0;
        const int steps = 25;
        double lattice_volume;

        Eigen::Matrix<double,-1,-1> HP_mat;
        Eigen::Matrix<double,-1,1> HP_v;

        void set_up(LatticeUtil::LatticeBasis Basis){
            dim_size = Basis.dim_size;
            lattice_volume = static_cast<double>(Basis.volume());            
        }

        void update_HP(const LatticeUtil::LatticePoint& LP,bool last = false){
            HP_mat.conservativeResize((List_size+1),dim_size);
            HP_v.conservativeResize((List_size+1),1);
            double e = 0;
            for(int i = 0;i<dim_size;i++) {
                HP_mat(List_size,i) = static_cast<double>(LP.vL[i]);
                e=e+(static_cast<double>(LP.vL[i])*static_cast<double>(LP.vL[i]));
            }
            HP_v(List_size,0) = e/2.0;
            List_size++;

            if((List_size%steps==0) || last) UpdateExperimentData();
            return;
        }

        void UpdateExperimentData(){
            List_size_vec.push_back(List_size);
            double volume = Calculate_Volume();
            Volume_ratio_vec.push_back(volume/lattice_volume);
            return;
        }

        
        //ここだけ変える
        double Calculate_Volume(){
            
            typedef Cartesian<double> Kernel;
            typedef typename Kernel::Point Point;
            typedef BoostRandomNumberGenerator<boost::mt19937, double, 3> RNGType;
            typedef HPolytope<Point> HPolytopeType;

            HPolytopeType HP(dim_size,HP_mat,HP_v);

            // Setup parameters for calculating volume
            int walk_len = 10 + HP.dimension()/10;
            double e = 0.1;
            auto volume = volume_cooling_balls
                <BallWalk, RNGType, HPolytopeType>(HP, e, walk_len).second;
                
            return volume;
            
            //return 23456.0;
        }
        

        void output(){
            std::string folderPATH = Util::EXfolderPATH() + "/VolumeApproximation";
            std::string filePATH = folderPATH + "/ExData" + std::to_string(0) + ".yaml";

            YAML::Node Exyaml;
            if(std::filesystem::exists(filePATH)) Exyaml = YAML::LoadFile(filePATH);
            YAML::Node ThisData;
            ThisData["dimension size"] = dim_size; 
            ThisData["List_size_vec"] = List_size_vec;
            ThisData["Volume_ratio_vec"] = Volume_ratio_vec;
            ThisData["List_size_vec"].SetStyle(YAML::EmitterStyle::Flow); 
            ThisData["Volume_ratio_vec"].SetStyle(YAML::EmitterStyle::Flow); 
            Exyaml.push_back(ThisData);

            YAML::Emitter emit;
            emit << Exyaml;
            std::ofstream yamlfile(filePATH);
            yamlfile << emit.c_str();
            yamlfile.close();
        }
    };

    struct FindLatticeVector_from_RandomSampling{
        int List_size = 0;
        int dim_size = 0;
        LatticeUtil::LatticeBasis Basis;

        Eigen::Matrix<double,-1,-1> HP_mat;
        Eigen::Matrix<double,-1,1> HP_v;

        void set_up(LatticeUtil::LatticeBasis _Basis){
            Basis = _Basis;
            dim_size = Basis.dim_size;       
        }

        void update_HP(const LatticeUtil::LatticePoint& LP){
            HP_mat.conservativeResize((List_size+1),dim_size);
            HP_v.conservativeResize((List_size+1),1);
            double e = 0;
            for(int i = 0;i<dim_size;i++) {
                HP_mat(List_size,i) = static_cast<double>(LP.vL[i]);
                e=e+(static_cast<double>(LP.vL[i])*static_cast<double>(LP.vL[i]));
            }
            HP_v(List_size,0) = e/2.0;
            List_size++;

            return;
        }

        LatticeUtil::LatticePoint FindCandidateLatticeVector(){
            LatticeUtil::EuclidPoint EP = RandomSampling_from_Polytope();
            return Basis.ApproxCVP(EP);
        }

        //ここが大変
        LatticeUtil::EuclidPoint RandomSampling_from_Polytope(){
            return LatticeUtil::EuclidPoint(0);
        }
    };

    struct ExperimentData{
        bool flag_CountingReptitions = true;
        bool flag_VolumeApproximation = false;
        bool flag_FindLatticeVector_from_RandomSampling = false;
        ExperimentTools::CountingReptitions CR;
        ExperimentTools::VolumeApproximation VA;
        ExperimentTools::FindLatticeVector_from_RandomSampling FLRS;
        

        ExperimentData(bool flag_CR = false,bool flag_VA = false,bool flag_FLRS = false){
            flag_CountingReptitions = flag_CR;
            flag_VolumeApproximation = flag_VA;
            flag_FindLatticeVector_from_RandomSampling = flag_FLRS;
        }
    };
}