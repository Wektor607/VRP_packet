from gurobipy import *
from gurobipy import Model, GRB, quicksum
import pandas as pd
import numpy as np
import pandas as pd
import requests

def try_request(url):
    while(1):
        try:
            r = requests.get(url)
            return r
        except:
            print("ERROR")

def distance_file(n, lst, lst_distance):
    for i in range(len(lst)):
        file_name = lst[i]
        file_dist = lst_distance[i]
        dist = pd.DataFrame(dict(
            x1 = [],
            y1 = [],
            x2 = [],
            y2 = [],
            distance = []))
        acnc = pd.read_csv(file_name, sep= "\t", error_bad_lines=False)
        j_start = 0
        for k in range(n):
            x1 = acnc.values[k][0]
            y1 = acnc.values[k][1]
            j = 0
            while(j < n):
                x2 = acnc.values[j][0]
                y2 = acnc.values[j][1]
                url = "http://router.project-osrm.org/route/v1/driving/{},{};{},{}".format(y1, x1, y2, x2)
                r = try_request(url)
                res = r.json()
                dist.loc[j_start, 'x1'] = x1
                dist.loc[j_start, 'y1'] = y1
                dist.loc[j_start, 'x2'] = x2
                dist.loc[j_start, 'y2'] = y2
                dist.loc[j_start, 'distance'] = res['routes'][0]['legs'][0]['distance']
                j += 1
                j_start += 1
        dist.to_csv(file_dist, index = False)

def main():
    lst = [
        "20(20 задач)/20201025_141430.csv",
        "20(20 задач)/20201025_142105.csv", 
        "20(20 задач)/20201101_150113.csv", 
        "20(20 задач)/20201115_132354.csv", 
        "20(20 задач)/20201116_232344.csv", 
        "20(20 задач)/20201224_222701.csv", 
        "20(20 задач)/20201225_204810.csv", 
        "20(20 задач)/20201228_230627.csv", 
        "20(20 задач)/20200925_093755.csv", 
        "20(20 задач)/20200927_100114.csv", 
        "20(20 задач)/20200928_114457.csv", 
        "20(20 задач)/20200930_163005.csv", 
        "20(20 задач)/20201005_160200.csv", 
        "20(20 задач)/20201008_165020.csv", 
        "20(20 задач)/20201014_154416.csv" 
    ]

    lst_distance = [
        "20(20 задач)/20201025_141430_dist.csv",
        "20(20 задач)/20201025_142105_dist.csv",
        "20(20 задач)/20201101_150113_dist.csv",
        "20(20 задач)/20201115_132354_dist.csv",
        "20(20 задач)/20201116_232344_dist.csv",
        "20(20 задач)/20201224_222701_dist.csv",
        "20(20 задач)/20201225_204810_dist.csv",
        "20(20 задач)/20201228_230627_dist.csv",
        "20(20 задач)/20200925_093755_dist.csv",
        "20(20 задач)/20200927_100114_dist.csv",
        "20(20 задач)/20200928_114457_dist.csv",
        "20(20 задач)/20200930_163005_dist.csv",
        "20(20 задач)/20201005_160200_dist.csv",
        "20(20 задач)/20201008_165020_dist.csv",
        "20(20 задач)/20201014_154416_dist.csv"
    ]
    print("Введите кол-во городов (21 или 51 или 101): ", end ='')
    # n = int(input())
    distance_file(n, lst, lst_distance)
    print("I'm create file!!")
    for name_file1, name_file2 in zip(lst, lst_distance):
        # Запишем все данные в виде массива списков
        data_file  = pd.read_csv(name_file1, sep="\t", error_bad_lines=True)
        arr_data   = data_file.values
        
        # Задаём объем грузовика
        Q = 30 # Для 20:30, 50:40, 100:50
        
        # Создаём списки координат и грузов для каждого клиента
        xc       = np.zeros(len(arr_data))
        yc       = np.zeros(len(arr_data))
        capacity = np.zeros(len(arr_data))
        w = 0
        for j in range(len(arr_data)):
            if(arr_data[j][2] <= Q):
                xc[w]       = arr_data[j][0]
                yc[w]       = arr_data[j][1]
                capacity[w] = arr_data[j][2]
                w += 1
        # Отдельно записываем координаты депо и исключаем их из списка координта и capacity клиентов       
        depo_xc  = xc[w-1]
        depo_yc  = yc[w-1]
        xc       = xc[:w-1]
        yc       = yc[:w-1]
        capacity = capacity[:w-1]
        
        # Исключаем депо из списка клиентов
        w -= 1
        
        #Создаем список клиентов
        clients = []   
        for i in range(0, w):
            clients.append(i)
        
        # Cоздаем список клиентов вместе с депо
        V = [len(arr_data) - 1] + clients

        A = [(i,j) for i in V for j in V if i != j] # possible arcs
        
        c = {}
        sheet_dist  = pd.read_csv(name_file2, sep=",", error_bad_lines=False)
        for i,j in A:
            # print((i,j))
            if(i == len(arr_data) - 1 and j <= max(A)[1]):
                for k in range(len(sheet_dist)):
                    if(depo_xc == sheet_dist.values[k][0] and depo_yc == sheet_dist.values[k][1] and xc[j] == sheet_dist.values[k][2] and yc[j] == sheet_dist.values[k][3]):
                        c[(i,j)] = sheet_dist.values[k][4]
                        break
            elif(i <= max(A)[1] and j == len(arr_data) - 1):
                for k in range(len(sheet_dist)):
                    if(xc[i] == sheet_dist.values[k][0] and yc[i] == sheet_dist.values[k][1] and depo_xc == sheet_dist.values[k][2] and depo_yc == sheet_dist.values[k][3]):
                        c[(i,j)] = sheet_dist.values[k][4]
                        break
            else:
                for k in range(len(sheet_dist)):
                    if(xc[i] == sheet_dist.values[k][0] and yc[i] == sheet_dist.values[k][1] and xc[j] == sheet_dist.values[k][2] and yc[j] == sheet_dist.values[k][3]):
                        c[(i,j)] = sheet_dist.values[k][4]
                        break
                        
        q = {i: capacity[i] for i in clients} # number of VRUs at location i to be picked
        q[len(arr_data) - 1] = 0
        time_limit = 200
        lst = []
        lst_time = []
        #-----------------------------------------------------------------------------------------------------------------#        
        model = Model('CVRP')

        # Declaration of variables
        x = model.addVars(A, vtype= GRB.BINARY)
        y = model.addVars(clients, vtype= GRB.CONTINUOUS)
        # setting the objective function
        model.modelSense = GRB.MINIMIZE
        model.setObjective(quicksum(x[i, j]*c[i, j] for i, j in A))
        # Adding constraints
        model.addConstrs(quicksum(x[i,j] for j in V if j!=i) == 1 for i in clients)
        model.addConstrs(quicksum(x[i,j] for i in V if i!=j) == 1 for j in clients)
        model.addConstrs((x[i,j] == 1) >> (y[i] + q[j] == y[j]) for (i,j) in A if i != len(arr_data) - 1 and j != len(arr_data) - 1)
        model.addConstrs(y[i] >= q[i] for i in clients)
        model.addConstrs(y[k] <= Q for k in clients)

        # Optimizing the model
        model.Params.TimeLimit = time_limit  # seconds
        model.Params.LogFile= "result_Branch_and_Cut_100.txt"
        model.optimize()
        if model.status == GRB.OPTIMAL:
            print('1.Optimal objective: %g' % model.objVal)
            print('Optimal cost: %g' % model.objVal)
            lst.append(model.objVal)
            lst_time.append(model.Runtime)
        elif model.status == GRB.INF_OR_UNBD:
            print('2.Model is infeasible or unbounded')
            res = -1
        elif model.status == GRB.INFEASIBLE:
            print('3.Model is infeasible')
            res = -1
        elif model.status == GRB.UNBOUNDED:
            print('4.Model is unbounded')
            res = -1
        else:
            print('5.Optimization ended with status %d' % model.status)
            print('Optimal cost: %g' % model.objVal)
            lst.append(model.objVal)
            lst_time.append(time_limit)

if __name__ == "__main__":
    main()