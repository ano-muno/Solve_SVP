from fpylll import *
import yaml
import os
import math

def norm(arr):
    ret = 0
    for e in arr:
        ret += e*e
    return math.sqrt(ret)

def GetShortestVectorNorm_usingLLL(_basis,dim_size):
    Basis =  IntegerMatrix(dim_size,dim_size)
    for i in range(dim_size):
        for j in range(dim_size):
            Basis[i,j] = _basis[i][j]

    _Basis_LLL = LLL.reduction(Basis)
    Basis_LLL = [[int(e) for e in _Basis_LLL_i] for _Basis_LLL_i in _Basis_LLL]

    return Basis_LLL


def GetInputData_fromYaml(yamlpath):
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
Basis_LLL = GetShortestVectorNorm_usingLLL(_basis,dim_size)

with open("../tmp/tmp_LLLinfo.yaml","w") as file:
    yaml.dump(dict({"LLLBasis" : list(Basis_LLL)}),file)