# from typing_extensions import Self
import vrp_c
from datetime import datetime
#import helloworld
# help(vrp_c)
#print(helloworld.parseOneTownPy("/Users/ivandruzkov/project/LKH0/20(20_задач)/20200925_093755.csv", "ttt/test0", 21, 60))

# cvrp.createBinFile("20(20 задач)/20200925_093755.csv", "/test", 21, 60)
# cvrp.parseOneTownPy("20(20 задач)/20200925_093755.csv", "ttt_cvrp/test0", 21)
# cvrp.modelMetaHeuristic("cvrp_lkh", "ttt_cvrp/test0", 21, 30)
parse_funcs = [vrp_c.parseOneTownPy]

class VRP:
    def __init__ (self, name_file, path_folder, count_towns, countTasks: int = 10000):
        self.name_file   = name_file 
        self.path_folder = path_folder 
        self.count_towns = count_towns
        self.count_Tasks = countTasks
        self.parse_file()
    def parse_file(self):
        print("Parse from VRP")

#TODO: запоминание маршрута
class CVRP (VRP):
    def __init__ (self, name_file, path_folder, count_towns, countTasks: int = 10000, capacity: int = 30):
        super().__init__(name_file, path_folder, count_towns, countTasks)
        self.capacity = capacity
    
    #TODO: ограничительная функция на все параметры
    
    def parse_file(self):
        # для SA и LKH моей реализации
        print("Parse from CVRP")
        vrp_c.parseOneTownPy(self.name_file, self.path_folder, self.count_towns)
        
    def sa(self, T: float = 1000, t_min: float = 10) -> [float, list]:
        vrp_c.modelMetaHeuristic("cvrp_sa", self.path_folder, self.count_towns, self.capacity)
        return [0.0, []]

    def lkh(self, name_opt: str = 'lkh3opt') -> [float, list]:
        pass

    def gurobi(self) -> [float, list]:
        pass

class CVRPTW (CVRP):
    def __init__ (self,  name_file, path_folder, count_towns, countTasks: int = 10000, capacity: int = 30, time_start: int = 0, time_end: int = 0):
        super().__init__(name_file, path_folder, count_towns, countTasks, capacity)
        self.time_start = time_start
        self.time_end   = time_end
    
    #TODO: ограничительная функция на все параметры
    
    def parse_file(self):
        # для SA и LKH моей реализации
        print("Parse from CVRPTW")
        vrp_c.parseOneTwTownPy(self.name_file, self.path_folder, self.count_towns)

    def sa(self, T: float = 1000, t_min: float = 10) -> [float, list]:
        pass
    
    def lkh(self, name_opt: str = 'lkh3opt') -> [float, list]:
        pass
    
    def gurobi(self) -> [float, list]:
        pass


def main():
    a = CVRP("20(20 задач)/20200925_093755.csv", "ttt_cvrp/test0", 21, 10000, 30)#, 600, 1500)
    a.sa()

if __name__ == "__main__":
    main()