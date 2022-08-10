import vrp_c
from datetime import datetime
import os
import numpy as np

class LP_OrTools:
    """
    Класс, в котором объявлены основные параметры, использующиеся в классах CVRP, CVRPTW:
        :type string name_file:   Название файла с данными;
        :type string path_folder: Название папки, в которой буду сохранены бинарные файлы с матрицами расстояний;
        :type int count_towns: Количество городов;
        :type int countTasks:  Количество итераций для решения одной оптимизационной задачи.
    """ 
    def __init__ (self, N, p, solver_name):
        self.N           = N
        self.p           = p 
        self.solver_name = solver_name
        
class CVRP (LP_OrTools):
    """
    Основной класс для решения задачи CVRP.
    """
    def __init__ (self, N, p, Q, solver_name):
        super().__init__(N, p, solver_name)
        self.Q = Q
    
   
    def LP_OrTools(self):
        vrp_c.modelMetaHeuristic(self.N, self.p, self.Q, self.solver_name)
        print("YES")
        return 0
  
    