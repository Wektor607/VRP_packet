from vrp_functions import *
from collections import deque
from datetime import datetime
def main():
    lst = [
        "20/20201025_141430.csv",
        "20/20201025_142105.csv", 
        "20/20201101_150113.csv", 
        "20/20201115_132354.csv", 
        "20/20201116_232344.csv", 
        "20/20201224_222701.csv", 
        # "20/20201225_204810.csv", 
        "20/20201228_230627.csv", 
        "20/20200925_093755.csv", 
        "20/20200927_100114.csv", 
        "20/20200928_114457.csv", 
        "20/20200930_163005.csv", 
        "20/20201005_160200.csv", 
        "20/20201008_165020.csv", 
        "20/20201014_154416.csv" 
    ]
    
    # TODO: сделать нормальное создание бинарников
    print('If you want open documentation write YES else press Enter')
    doc = input()
    if(doc == 'YES'):
        help(vrp_c) #TODO: в отредактировать текст в документации
    print('What do you want to solve: CVRP or CVRPTW?')
    task = input()
    if(task == 'CVRP'):
        print('which method do you want to use to optimize the routse: SA or LKH or Gurobi?')
        method = input()
        if(method == 'SA'):
            for i in lst:
                with open(i) as f:
                    count_towns = sum(1 for _ in f) - 1
                if(count_towns == 21):
                    max_capacity = 30
                elif(count_towns == 51):
                    max_capacity = 40
                elif(count_towns == 101):
                    max_capacity = 50
                a = CVRP(i, "ttt_cvrp/test0", count_towns, 10000, max_capacity) #TODO: некоторые параметры брать автоматически из файла
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
                with open(i) as f:
                    count_towns = sum(1 for _ in f) - 1
                if(count_towns == 21):
                    max_capacity = 30
                elif(count_towns == 51):
                    max_capacity = 40
                elif(count_towns == 101):
                    max_capacity = 50
                a = CVRP(i, "ttt_cvrp/test0", count_towns, 10000, max_capacity) #TODO: некоторые параметры брать автоматически из файла
                print(a.lkh(name_opt))
                with open(f'LKH_{name_opt[3:]}_CVRP_result.txt', 'r') as res_file:
                    data = res_file.read()
                    with open('LKH_RES_CVRP.txt', 'a') as write_file:
                        write_file.write(data)
        else:
            Gurobi(21)
            pass #TODO: доделать для Gurobi
    else:
        print('which method do you want to use to optimize the routse: SA or LKH or Gurobi?')
        method = input()
        if(method == 'SA'):
            for i in lst:
                with open(i) as f:
                    count_towns = sum(1 for _ in f) - 1
                with open(i) as f:
                    [last_line] = deque(f, maxlen=1)
                    last_line = last_line.split('\t')[3].split('-')
                if(count_towns == 21):
                    max_capacity = 500
                elif(count_towns == 51):
                    max_capacity = 750
                elif(count_towns == 101):
                    max_capacity = 1000
                a = CVRPTW(i, "ttt_cvrp/test0", count_towns, 10000, max_capacity, int(last_line[0].split(':00')[0]) * 60, int(last_line[1].split(':00')[0]) * 60) #TODO: некоторые параметры брать автоматически из файла
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
                with open(i) as f:
                    count_towns = sum(1 for _ in f) - 1
                with open(i) as f:
                    [last_line] = deque(f, maxlen=1)
                    last_line = last_line.split('\t')[3].split('-')                
                if(count_towns == 21):
                    max_capacity = 500
                elif(count_towns == 51):
                    max_capacity = 750
                elif(count_towns == 101):
                    max_capacity = 1000
                a = CVRPTW(i, "ttt_cvrp/test0", count_towns, 10000, max_capacity, int(last_line[0].split(':00')[0]) * 60, int(last_line[1].split(':00')[0]) * 60) #TODO: некоторые параметры брать автоматически из файла
                print(a.lkh(name_opt))
                with open(f'LKH_{name_opt[3:]}_CVRPTW_result.txt', 'r') as res_file:
                    data = res_file.read()
                    with open('LKH_RES_CVRPTW.txt', 'a') as write_file:
                        write_file.write(data)
        else:
            pass #TODO: доделать для Gurobi
            
if __name__ == "__main__":
    main()