from fpylll import *
import yaml
import os
import math

def norm(arr):
    ret = 0
    for e in arr:
        ret += e*e
    return math.sqrt(ret)

def GetShortestVectorNorm_usingBKZ(_basis,dim_size):
    Basis =  IntegerMatrix(dim_size,dim_size)
    for i in range(dim_size):
        for j in range(dim_size):
            Basis[i,j] = _basis[i][j]

    par =  BKZ.Param(dim_size)
    BB = BKZ.reduction(Basis, par)
    sv = [int(e) for e in BB[0]]

    return norm(BB[0]),sv


def GetInputData_fromYaml(yamlpath):
    #cwd = os.getcwd()
    #yamlpath = cwd + "/data/SVP/Data" + str(Identification_Number) + ".yaml" 
    dim_size = -1
    _basis = []

    with open(yamlpath) as file:
        yml = yaml.safe_load(file)
        dim_size = int(yml["InputData"]["dim_size"])
        _basis = yml["InputData"]["LatticeBasis"]

    return _basis,dim_size

import sys
yaml_path = sys.argv[1]
_basis,dim_size = GetInputData_fromYaml(yaml_path)
mu,sv = GetShortestVectorNorm_usingBKZ(_basis,dim_size)

with open("../tmp/tmp_SVinfo.yaml","w") as file:
    yaml.dump(dict({"shortestvector norm" : mu , "shortestvector" : list(sv)}),file)

