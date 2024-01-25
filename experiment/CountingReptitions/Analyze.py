import yaml
import matplotlib.pyplot as plt
import numpy as np

def Analyze(min_dim :int,max_dim : int):
    with open('ExData0.yaml', 'r') as file:
        datas = yaml.safe_load(file)

    mymap = dict({})
    mymap_collision = dict({})
    for i,data in enumerate(datas):
        nowdim = int(data["dimension size"])
        if nowdim in list(mymap.keys()):
           mymap[nowdim] = np.append(mymap[nowdim],data["Total Reptitions"])
           mymap_collision[nowdim] = np.append(mymap_collision[nowdim],data["collision Times"])
        else:
            mymap[nowdim] = np.array(data["Total Reptitions"])
            mymap_collision[nowdim] = np.array(data["collision Times"])
    
    x_max = [i for i in range(min_dim,max_dim+1)]
    y_max = [np.log2(float(np.max(mymap[i]))) for i in range(min_dim,max_dim+1)]

    x_mean = [i for i in range(min_dim,max_dim+1)]
    y_mean = [np.log2(float(np.mean(mymap[i]))) for i in range(min_dim,max_dim+1)]

    from sklearn.linear_model import LinearRegression

    model = LinearRegression()
    x_max_manuf = np.array(x_max).reshape(-1, 1)
    model.fit(x_max_manuf, y_max)
    slope = model.coef_
    print("傾き（係数）(max):", slope)
    print(y_max)
    plt.plot(x_max,y_max, marker='o', linestyle='-',color='r', label='max')

    model = LinearRegression()
    x_mean_manuf = np.array(x_mean).reshape(-1, 1)
    model.fit(x_mean_manuf, y_mean)
    slope = model.coef_
    print("傾き（係数）(mean):", slope)
    print(y_mean)
    plt.plot(x_mean,y_mean, marker='o', linestyle='-',color='b', label='mean')

    plt.xlabel('Dimension')
    plt.ylabel('log2(Repetition Count)')
    plt.xticks(x_max)
    plt.xticks(x_mean)
    plt.legend()
    plt.show()

    x = [i for i in range(min_dim,max_dim+1)]
    y = [np.mean(mymap_collision[i]) for i in range(min_dim,max_dim+1)]
    plt.close()
    plt.plot(x,y)
    plt.xlabel('Dimension')
    plt.ylabel('Collision Times')
    plt.xticks(x)
    plt.show()


print("Input min_dim and max_dim")
min_dim,max_dim = map(int, input().split())
Analyze(min_dim,max_dim)