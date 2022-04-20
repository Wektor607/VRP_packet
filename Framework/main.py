from functions import *
from collections import deque
from datetime import datetime
def main():
    lst = [
        "100(20 задач)/20201020_182616.csv",
        "100(20 задач)/20201021_185504.csv",
        "100(20 задач)/20201111_223641.csv",
        "100(20 задач)/20201118_223519.csv",
        "100(20 задач)/20201119_230826.csv",
        "100(20 задач)/20201123_210620.csv",
        "100(20 задач)/20201125_032739.csv",
        "100(20 задач)/20201125_225552.csv",
        "100(20 задач)/20201126_190637.csv",
        "100(20 задач)/20201130_210949.csv",
        "100(20 задач)/20201201_165823.csv",
        "100(20 задач)/20201202_191224.csv",
        "100(20 задач)/20201208_202137.csv",
        "100(20 задач)/20201215_221946.csv"
        # "50/20201007_185322.csv",
        # "50/20201023_203450.csv",
        # "50/20201027_214904.csv",
        # "50/20201028_202211.csv",
        # "50/20201105_201910.csv",
        # "50/20201109_190333.csv",
        # "50/20201110_211510.csv",
        # "50/20201117_214239.csv",
    ]
    # "100(20 задач)/20201020_182616.csv",
    # "100(20 задач)/20201021_185504.csv",
    # "100(20 задач)/20201111_223641.csv",
    # "100(20 задач)/20201118_223519.csv",
    # "100(20 задач)/20201119_230826.csv",
    # "100(20 задач)/20201123_210620.csv",
    # "100(20 задач)/20201125_032739.csv",
    # "100(20 задач)/20201125_225552.csv",
    # "100(20 задач)/20201126_190637.csv",
    # "100(20 задач)/20201130_210949.csv",
    # "100(20 задач)/20201201_165823.csv",
    # "100(20 задач)/20201202_191224.csv",
    # "100(20 задач)/20201208_202137.csv",
    # "100(20 задач)/20201215_221946.csv",
    
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
                    iteretions = 10000
                elif(count_towns == 51):
                    max_capacity = 40
                    iteretions = 30000
                elif(count_towns == 101):
                    max_capacity = 50
                    iteretions = 50000
                a = CVRP(i, "ttt_cvrp/test0", count_towns, iteretions, max_capacity) #TODO: некоторые параметры брать автоматически из файла
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
                    iteretions = 10000
                elif(count_towns == 51):
                    max_capacity = 40
                    iteretions = 30000
                elif(count_towns == 101):
                    max_capacity = 50
                    iteretions = 50000
                a = CVRP(i, "ttt_cvrp/test0", count_towns, iteretions, max_capacity) #TODO: некоторые параметры брать автоматически из файла
                print(a.lkh(name_opt))
                with open(f'LKH_{name_opt[3:]}_CVRP_result.txt', 'r') as res_file:
                    data = res_file.read()
                    with open('LKH_RES_CVRP.txt', 'a') as write_file:
                        write_file.write(data)
        else:
            for i in lst:
                with open(i) as f:
                    count_towns = sum(1 for _ in f) - 1
                if(count_towns == 21):
                    max_capacity = 30
                    iteretions = 10000
                elif(count_towns == 51):
                    max_capacity = 40
                    iteretions = 30000
                elif(count_towns == 101):
                    max_capacity = 50
                    iteretions = 50000
                a = CVRP(i, "ttt_cvrp/test0", count_towns, iteretions, max_capacity) #TODO: некоторые параметры брать автоматически из файла
                print(a.gurobi())
            p = count_towns - 1
            infile = open(f'result_Branch_and_Cut_{p}.txt',  'r')
            outfile = open(f'result_Gurobi_{p}_CVRP.txt',  'w')

            copy = False
            prev_content = '10000000000000000000000000000000000000000000'
            for line in infile:
                if 'Expl Unexpl |  Obj  Depth IntInf | Incumbent    BestBd   Gap | It/Node Time' in line:
                    copy = True
                elif 'Cutting planes:' in line:
                    copy = False
                elif copy:
                    content = line.split()
                    new_line = ''
                    if len(content) == 10:
                        if(float(content[5]) > float(prev_content)):
                            outfile.write('\n')
                        new_line += content[5]
                        prev_content = content[5]
                    elif len(content) == 8:
                        if(float(content[3]) > float(prev_content)):
                            outfile.write('\n')
                        new_line += content[3]
                        prev_content = content[3]

                    if len(content) != 0:
                        new_line += '\t' + content[-1] 
                        new_line = new_line[:-1] + '\n'
                        if(new_line[0] != '\t'):
                            outfile.write(new_line)
            infile.close()
            os.remove(f'result_Branch_and_Cut_{p}.txt')
            outfile.close()
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
                    iteretions = 10000
                elif(count_towns == 51):
                    max_capacity = 750
                    iteretions = 100000
                elif(count_towns == 101):
                    max_capacity = 1000
                    iteretions = 50000
                start = int(last_line[0].split(':00')[0]) * 60
                end   = int(last_line[1].split(':00')[0]) * 60
                a = CVRPTW(i, "ttt_cvrp/test0", count_towns, iteretions, max_capacity, start, end) #TODO: некоторые параметры брать автоматически из файла
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
                    iteretions = 10000
                elif(count_towns == 51):
                    max_capacity = 750
                    iteretions = 30000
                elif(count_towns == 101):
                    max_capacity = 1000
                    iteretions = 50000
                start = int(last_line[0].split(':00')[0]) * 60
                end   = int(last_line[1].split(':00')[0]) * 60
                a = CVRPTW(i, "ttt_cvrp/test0", count_towns, iteretions, max_capacity, start, end) #TODO: некоторые параметры брать автоматически из файла
                print(a.lkh(name_opt))
                with open(f'LKH_{name_opt[3:]}_CVRPTW_result.txt', 'r') as res_file:
                    data = res_file.read()
                    with open('LKH_RES_CVRPTW.txt', 'a') as write_file:
                        write_file.write(data)
        else:
            pass #TODO: доделать для Gurobi
            
if __name__ == "__main__":
    main()