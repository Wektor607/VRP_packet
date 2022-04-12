# from typing_extensions import Self
import vrp_c
from datetime import datetime
#import helloworld
# help(vrp_c)

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
def parse_dist_and_tour():
    max_count = -1
    mas_dist = []
    with open('res_distance.txt', 'r') as read_file:
        data = read_file.read().split('@')[:-1]
        for i in range(len(data)-1, 0, -2):
            data[i] = data[i].replace('#', '')
            if(float(data[i]) != -1):
                max_count = float(data[i])
                mas_dist = list(map(lambda t: list(map(int, t.split(' '))), data[i-1][:-1].split('#')))
                break 
    return [max_count, mas_dist]    
        

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
        return parse_dist_and_tour()

    def lkh(self, name_opt: str = 'lkh3opt') -> [float, list]:
        if(name_opt == 'lkh2opt'):
            vrp_c.modelMetaHeuristic("cvrp_lkh_2opt", self.path_folder, self.count_towns, self.capacity)
        else:
            vrp_c.modelMetaHeuristic("cvrp_lkh_3opt", self.path_folder, self.count_towns, self.capacity)
        return parse_dist_and_tour()

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
        vrp_c.modelMetaHeuristic("cvrptw_sa", self.path_folder, self.count_towns, self.capacity)
        return parse_dist_and_tour()
    
    def lkh(self, name_opt: str = 'lkh3opt') -> [float, list]:
        if(name_opt == 'lkh2opt'):
            vrp_c.modelMetaHeuristic("cvrptw_lkh_2opt", self.path_folder, self.count_towns, self.capacity)
        else:
            vrp_c.modelMetaHeuristic("cvrptw_lkh_3opt", self.path_folder, self.count_towns, self.capacity)
        return parse_dist_and_tour()
    
    def gurobi(self) -> [float, list]:
        pass

#TODO: Расстояния в CVRP и CVRPTW либо перевести в метры, либо в киллометры 
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
        # "20/20200927_100114.csv", 
        # "20/20200928_114457.csv", 
        # "20/20200930_163005.csv", 
        # "20/20201005_160200.csv", 
        # "20/20201008_165020.csv", 
        # "20/20201014_154416.csv" 
    ]
    
    with open('LKH_RES.txt', 'w') as write_file:
        write_file.write('')

    # TODO: сделать нормальное создание бинарников
    # help(vrp_c) #TODO: доделать документацию
    print('What do you want to solve: CVRP or CVRPTW?')
    task = input()
    if(task == 'CVRP'):
        print('which method do you want to use to optimize the routse: SA or LKH or Gurobi?')
        method = input()
        if(method == 'SA'):
            for i in lst:
                a = CVRP(i, "ttt_cvrp/test0", 21, 10000, 30) #TODO: некоторые параметры брать автоматически из файла
                print(a.sa())
                with open(f'SA_CVRP_result.txt', 'r') as res_file:
                    data = res_file.read()
                    with open('SA_RES_CVRP.txt', 'a') as write_file:
                        write_file.write(data)
        elif(method == 'LKH'):
            while(1):
                print("Input lkh2opt or lkh3opt:")
                name_opt = input()
                if(name_opt == 'lkh2opt' or name_opt == 'lkh3opt'):
                    break
                else:
                    print('Try again!')
            
            for i in lst:
                a = CVRP(i, "ttt_cvrp/test0", 21, 10000, 30) #TODO: некоторые параметры брать автоматически из файла
                print(a.lkh(name_opt))
                with open(f'LKH_{name_opt[3:]}_CVRP_result.txt', 'r') as res_file:
                    data = res_file.read()
                    with open('LKH_RES_CVRP.txt', 'a') as write_file:
                        write_file.write(data)
        else:
            pass #TODO: доделать для Gurobi
    else:
        print('which method do you want to use to optimize the routse: SA or LKH or Gurobi?')
        method = input()
        if(method == 'SA'):
            for i in lst:
                a = CVRPTW(i, "ttt_cvrp/test0", 21, 10000, 30, 600, 1500) #TODO: некоторые параметры брать автоматически из файла
                print(a.sa())
                with open(f'SA_CVRPTW_result.txt', 'r') as res_file:
                    data = res_file.read()
                    with open('SA_RES_CVRPTW.txt', 'a') as write_file:
                        write_file.write(data)
        elif(method == 'LKH'):
            while(1):
                print("Input lkh2opt or lkh3opt:")
                name_opt = input()
                if(name_opt == 'lkh2opt' or name_opt == 'lkh3opt'):
                    break
                else:
                    print('Try again!')
            
            for i in lst:
                a = CVRPTW(i, "ttt_cvrp/test0", 21, 10000, 30, 600, 1500) #TODO: некоторые параметры брать автоматически из файла
                print(a.lkh(name_opt))
                with open(f'LKH_{name_opt[3:]}_CVRPTW_result.txt', 'r') as res_file:
                    data = res_file.read()
                    with open('LKH_RES_CVRPTW.txt', 'a') as write_file:
                        write_file.write(data)
        else:
            pass #TODO: доделать для Gurobi
            
if __name__ == "__main__":
    main()