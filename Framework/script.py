# %matplotlib inline
# import mpld3
# mpld3.enable_notebook()
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
class graphic_vrp:
    def parse_file(name_file):
        with open(name_file, 'r') as f:
#             lst = list(map(lambda t: list(map(lambda t: None if t == '\n' else float(t),t.split('\t'))), f.readlines()))
#             intepolate = []
            cost = [[]]
            time = [[]]
            u = 0
            for i in f.readlines():
                if(i == '\n'):
                    cost.append([])
                    time.append([])
                    u += 1
                else:
                    var = i.split('\t')
                    if(float(var[1]) not in time):
                        cost[u].append(float(var[0]))
                        time[u].append(float(var[1]))
            interpolate = []
            for i in range(len(cost)):
                interpolate.append(interp1d(time[i], cost[i], kind='linear'))
            return interpolate, time, cost
        exit(-1)
            
spisok_gurobi, time_gurobi, cost_gurobi = graphic_vrp.parse_file("Gurobi_50_CVRP.txt")
spisok_lkh, time_lkh, cost_lkh = graphic_vrp.parse_file("LKH2opt_50_CVRP.txt")
spisok_sa, time_sa, cost_sa    = graphic_vrp.parse_file("SA_50_CVRP.txt")

x = np.linspace(0, 100, num=101, endpoint=True)
gurobi_mean = np.zeros(len(x)) 
lkh_mean    = np.zeros(len(x))
sa_mean     = np.zeros(len(x))
for i in range(min(len(spisok_lkh), len(spisok_sa))):
    gurobi = np.array(spisok_gurobi[i](x))
    lkh    = np.array(spisok_lkh[i](x))
    sa     = np.array(spisok_sa[i](x))
    gurobi_mean += gurobi
    lkh_mean += lkh
    sa_mean += sa
    
gurobi_mean /= len(spisok_gurobi)
lkh_mean    /= len(spisok_lkh)
sa_mean     /= len(spisok_sa)
gap1 = (abs(sa_mean-gurobi_mean) / gurobi_mean) * 100
gap2 = (abs(lkh_mean-gurobi_mean) / gurobi_mean) * 100
colors = ['#E69F00', '#56B4E9', '#F0E442']
plt.hist([sa_mean, gurobi_mean, lkh_mean], 100, color = colors)
# plt.hist(gurobi_mean, 100, density=True, facecolor='g', alpha=0.75)
# plt.hist(lkh_mean, 100, density=True, facecolor='g', alpha=0.75)
# gap2 = (abs(gurobi_mean-lkh_mean) / lkh_mean) * 100
# plt.plot(x, gap1, x, gap2)
plt.show()