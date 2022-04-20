import vrp_c
import gurobi_functions as gur_f
from gurobipy import *
from gurobipy import Model, GRB, quicksum
from datetime import datetime
import os
import numpy as np
import pandas as pd

class VRP:
    """
    Класс, в котором объявлены основные параметры, использующиеся в классах CVRP, CVRPTW:
        :type string name_file:   Название файла с данными;
        :type string path_folder: Название папки, в которой буду сохранены бинарные файлы с матрицами расстояний;
        :type int count_towns: Количество городов;
        :type int countTasks:  Количество итераций для решения одной оптимизационной задачи.
    """ 
    def __init__ (self, name_file, path_folder, count_towns, countTasks: int = 10000):
        self.name_file   = name_file 
        self.path_folder = path_folder 
        self.count_towns = count_towns
        self.count_Tasks = countTasks
        self.parse_file()

#TODO: запоминание маршрута
def parse_dist_and_tour():
    """
    Функция, возвращающая список из 2 элементов:
        :type float max_count: Длина (в метрах) оптимизированного маршрута;
        :type list mas_dist:  Список подмаршрута оптимизированного маршрута.
    """
    max_count = -1
    mas_dist = []
    with open('res_distance.txt', 'r') as read_file:
        data = read_file.read().split('@')[:-1]
        for i in range(len(data)-1, 0, -2):
            if(float(data[i]) != -1):
                max_count = float(data[i])
                mas_dist = list(map(lambda t: list(map(int, t.split(' '))), data[i-1][:-1].split('#')))
                break 
    return [max_count, mas_dist]    
        


class CVRP (VRP):
    """
    Основной класс для решения задачи CVRP.
    """
    def __init__ (self, name_file, path_folder, count_towns, countTasks: int = 10000, capacity: int = 30):
        super().__init__(name_file, path_folder, count_towns, countTasks)
        self.capacity = capacity
    
    #TODO: ограничительная функция на все параметры
    
    def parse_file(self):
        """
        Функция, создающая матрицы расстояний, которые записываются в бинарные файлы для задачи CVRP. На вход подаются следующие параметры:
            :type string name_file:   Имя файла с данными;
            :type string path_folder: Название папки, в которой буду сохранены бинарные файлы с матрицами расстояний;
            :type int count_towns: Количество городов.
        """
        # для SA и LKH моей реализации
        print("Parse from CVRP")
        # gur_f.distance_file(self.count_towns, [self.name_file], [self.name_file[ :self.name_file.rfind('.csv')] + '_dist.csv']) #TODO: сделать только для Gurobi
        vrp_c.parseOneTownPy(self.name_file, self.path_folder, self.count_towns)
   
    def sa(self, T: float = 1000, t_min: float = 10) -> [float, list]:
        """
        Функция, вызывающая алгоритм "Имитации отжига" для решения задачи CVRP. На вход подается два параметра:
            :type float T:     начальная температура, которая с течением времени убывает;
            :type float t_min: конечная температура, до которой опускается температура T.
        """ 
        vrp_c.modelMetaHeuristic("cvrp_sa", self.path_folder, self.count_towns, self.capacity)
        return parse_dist_and_tour()
  
    def lkh(self, name_opt: str = 'lkh3opt') -> [float, list]:
        """
        Функция, вызывающая алгоритм "Эвристика Лина-Кёрнигана" для решения задачи CVRP. На вход подается один параметр.
            :type string name_opt: название метода оптимизации: 2-opt, 3-opt. 
        """
        if(name_opt == 'lkh2opt'):
            vrp_c.modelMetaHeuristic("cvrp_lkh_2opt", self.path_folder, self.count_towns, self.capacity)
        else:
            vrp_c.modelMetaHeuristic("cvrp_lkh_3opt", self.path_folder, self.count_towns, self.capacity)
        return parse_dist_and_tour()

    def gurobi(self) -> [float, list]: #TODO: доделать для Gurobi
        # Запишем все данные в виде массива списков
        data_file  = pd.read_csv(self.name_file, sep="\t", error_bad_lines=True)
        arr_data   = data_file.values
        
        # Задаём объем грузовика
        Q = self.capacity # Для 20:30, 50:40, 100:50
        
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
        sheet_dist  = pd.read_csv(self.name_file[ :self.name_file.rfind('.csv')] + '_dist.csv', sep=",", error_bad_lines=False)
        for i,j in A:
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
        time_limit = 100
        
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
        model.Params.LogFile= f'result_Branch_and_Cut_{w}.txt'
        model.optimize()
        if model.status == GRB.OPTIMAL:
            print('1.Optimal objective: %g' % model.objVal)
            print('Optimal cost: %g' % model.objVal)
        elif model.status == GRB.INF_OR_UNBD:
            print('2.Model is infeasible or unbounded')
        elif model.status == GRB.INFEASIBLE:
            print('3.Model is infeasible')
        elif model.status == GRB.UNBOUNDED:
            print('4.Model is unbounded')
        else:
            print('5.Optimization ended with status %d' % model.status)
            print('Optimal cost: %g' % model.objVal)

class CVRPTW (VRP):
    """
    Это основной класс для решения задачи CVRPTW
    """
    def __init__ (self,  name_file, path_folder, count_towns, countTasks: int = 10000, capacity: int = 30, time_start: int = 0, time_end: int = 0):
        super().__init__(name_file, path_folder, count_towns, countTasks)
        self.capacity = capacity
        self.time_start = time_start
        self.time_end   = time_end
    
    #TODO: ограничительная функция на все параметры
    
    def parse_file(self):
        """
        Функция, создающая матрицы расстояний, которые записываются в бинарные файлы для задачи CVRPTW. На вход подаются следующие параметры:
            :type string name_file:   Имя файла с данными;
            :type string path_folder: Название папки, в которой буду сохранены бинарные файлы с матрицами расстояний;
            :type int count_towns: Количество городов.
        """
        # для SA и LKH моей реализации
        print("Parse from CVRPTW")
        # vrp_c.parseOneTwTownPy(self.name_file, self.path_folder, self.count_towns)

    def sa(self, T: float = 1000, t_min: float = 10) -> [float, list]:
        """
        Функция, вызывающая алгоритм "Имитации отжига" для решения задачи CVRPTW. На вход подается два параметра:
            :type float T:     начальная температура, которая с течением времени убывает;
            :type float t_min: конечная температура, до которой опускается температура T.
        """ 
        vrp_c.modelMetaHeuristic("cvrptw_sa", self.path_folder, self.count_towns, self.capacity)
        return parse_dist_and_tour()
    
    def lkh(self, name_opt: str = 'lkh3opt') -> [float, list]:
        """
        Функция, вызывающая алгоритм "Эвристика Лина-Кёрнигана" для решения задачи CVRP. На вход подается один параметр.
            :type string name_opt: название метода оптимизации: 2-opt, 3-opt.
        """
        if(name_opt == 'lkh2opt'):
            vrp_c.modelMetaHeuristic("cvrptw_lkh_2opt", self.path_folder, self.count_towns, self.capacity)
        else:
            vrp_c.modelMetaHeuristic("cvrptw_lkh_3opt", self.path_folder, self.count_towns, self.capacity)
        return parse_dist_and_tour()
    
    def gurobi(self) -> [float, list]:
        pass

