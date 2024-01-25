import yaml
import matplotlib.pyplot as plt
import numpy as np

def Analyze(dimv : list):
    color = ['r','b','g']

    with open('ExData0.yaml', 'r') as file:
        datas = yaml.safe_load(file)

    sum_final_ratio = 0.0
    for i,dim in enumerate(dimv):
        cnt = 0
        sum_final_ratio = 0
        for data in datas:
            if int(data["dimension size"]) != dim:
                continue
            indices = data["List_size_vec"]
            volume_ratios = data["Volume_ratio_vec"]
            indices_available = []
            volume_ratios_log2 = []
            for j,e in enumerate(volume_ratios):
                if float(e) >1:
                    volume_ratios_log2.append(np.log2(float(e)))
                    indices_available.append(indices[j])
            #print(volume_ratios_log2)
            #sum_final_ratio += volume_ratios_log2[-1]
            if cnt==0:
                plt.plot(indices_available,volume_ratios_log2, label='dim' + str(dim),color = color[1])
            else:
                plt.plot(indices_available,volume_ratios_log2,color = color[1])
            cnt+=1
        #print(sum_final_ratio/cnt)
        
    plt.xlabel('List Size')
    plt.ylabel('log2(Vol(k(L))/Vol(L(B)))')    
    plt.legend()
    plt.show()

print("Input dimendion size list")
dim_size = list(map(int, input().split()))
Analyze(dim_size)