# from typing_extensions import Self
import vrp_c
from gurobi_CVRP import Gurobi
from datetime import datetime
import os
#import helloworld
# help(vrp_c)

parse_funcs = [vrp_c.OpenDocumentation]

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
        self.parse_file() # РАСКОМЕНТИТЬ

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
        # Gurobi(21)
        pass

class CVRPTW (CVRP):
    """
    Это основной класс для решения задачи CVRPTW
    """
    def __init__ (self,  name_file, path_folder, count_towns, countTasks: int = 10000, capacity: int = 30, time_start: int = 0, time_end: int = 0):
        super().__init__(name_file, path_folder, count_towns, countTasks, capacity)
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
        vrp_c.parseOneTwTownPy(self.name_file, self.path_folder, self.count_towns)

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