from sklearn import preprocessing
from functions import *
from collections import deque
from datetime import datetime
from list import *
from preprocessing_data import *
def main():
    print('If you want open documentation write YES else press Enter')
    doc = input()
    if(doc == 'YES'):
        help(vrp_c) #TODO: в отредактировать текст в документации
    print('What do you want to solve: CVRP or CVRPTW?')
    task = input()
    preprocess(task)
    if(task == 'CVRP'):
        print('which method do you want to use to optimize the routse: SA or LKH or Gurobi?')
        method = input()
        idx = 0 
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
                a = CVRP(i, f"20_cvrp/test{idx}", count_towns, iteretions, max_capacity) #TODO: некоторые параметры брать автоматически из файла
                idx += 1
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
                a = CVRP(i, f"cvrp/test{idx}", count_towns, iteretions, max_capacity) #TODO: некоторые параметры брать автоматически из файла
                idx += 1
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
                a = CVRP(i, f"cvrp/test{idx}", count_towns, iteretions, max_capacity) #TODO: некоторые параметры брать автоматически из файла
                idx += 1
                print(a.gurobi())
            p = count_towns - 1
            infile = open(f'result_Branch_and_Cut_CVRP_{p}.txt',  'r')
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
            os.remove(f'result_Branch_and_Cut_CVRP_{p}.txt')
            outfile.close()
    elif(task == 'CVRPTW'):
        print('which method do you want to use to optimize the routse: SA or LKH or Gurobi?')
        method = input()
        idx = 1 
        if(method == 'SA'):
            for i in lst:
                with open(i) as f:
                    count_towns = sum(1 for _ in f) - 1
                with open(i) as f:
                    [last_line] = deque(f, maxlen=1)
                    last_line = last_line.split('\t')[3].split('-')
                if(count_towns == 21):
                    max_capacity = 500
                    iteretions = 700
                elif(count_towns == 51):
                    max_capacity = 750
                    iteretions = 1250
                elif(count_towns == 101):
                    max_capacity = 1000
                    iteretions = 2500
                start = (int(last_line[0].split(':')[0]) + int(last_line[0].split(':')[1])) * 60
                end   = (int(last_line[1].split(':')[0]) + int(last_line[1].split(':')[1])) * 60
                a = CVRPTW(i, f"20_tw/test{idx}", count_towns, iteretions, max_capacity, start, end) #TODO: некоторые параметры брать автоматически из файла
                idx += 1
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
                    iteretions = 700
                elif(count_towns == 51):
                    max_capacity = 750
                    iteretions = 1250
                elif(count_towns == 101):
                    max_capacity = 1000
                    iteretions = 300
                start = (int(last_line[0].split(':')[0]) + int(last_line[0].split(':')[1])) * 60
                end   = (int(last_line[1].split(':')[0]) + int(last_line[1].split(':')[1])) * 60
                a = CVRPTW(i, f"100_tw/test{idx}", count_towns, iteretions, max_capacity, start, end) #TODO: некоторые параметры брать автоматически из файла
                idx += 1
                print(a.lkh(name_opt))
                with open(f'LKH_{name_opt[3:]}_CVRPTW_result.txt', 'r') as res_file:
                    data = res_file.read()
                    with open('LKH_RES_CVRPTW.txt', 'a') as write_file:
                        write_file.write(data)
        else:
            for i in lst:
                with open(i) as f:
                    count_towns = sum(1 for _ in f) - 1
                if(count_towns == 21):
                    max_capacity = 500
                    iteretions = 700
                elif(count_towns == 51):
                    max_capacity = 750
                    iteretions = 1250
                elif(count_towns == 101):
                    max_capacity = 1000
                    iteretions = 2500
                a = CVRPTW(i, f"50_tw/test{idx}", count_towns, iteretions, max_capacity) #TODO: некоторые параметры брать автоматически из файла
                idx += 1
                print(a.gurobi())
            p = count_towns - 1
            infile = open(f'result_Branch_and_Cut_CVRPTW_{p}.txt',  'r')
            outfile = open(f'result_Gurobi_{p}_CVRPTW.txt',  'w')

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
            os.remove(f'result_Branch_and_Cut_CVRPTW_{p}.txt')
            outfile.close()
            
if __name__ == "__main__":
    main()