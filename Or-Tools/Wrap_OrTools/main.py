from collections import deque
from datetime import datetime
import matplotlib.pyplot as plt
from functions import *

def main():
    N = 50
    p = 40
    maxCapacity = 100
    solver_name = 'SCIP'
    a = CVRP(N, p, maxCapacity, solver_name)
    print(a.LP_OrTools())
    
    q = open('coords.txt')
    xcoord = []
    ycoord = []
    for line in q:
        p = line.split()
        xcoord.append(float(p[0]))
        ycoord.append(float(p[1]))
    
    f = open('result.txt')
    res = dict()
    for line in f:
        p = line.split()
        res.update({(int(p[0]), int(p[1])): 1.0})

    # Paint

    plt.figure(figsize=(12,8))
    plt.scatter(xcoord, ycoord, color="red")

    plt.scatter(xcoord[0], ycoord[0], color="green", marker='D')
    plt.annotate("Depo", (xcoord[0]-1, ycoord[0]-5.5))

    clients = [i for i in range(N+1) if i != 0]
    for i in clients:
        plt.annotate('${%d}$' % i, (xcoord[i]-1, ycoord[i]-5))

    for i in range(1, N+1): 
        tmp = 0 
        for j in range(1, N+1): 
            if ((i, j) in res) and i != j and res[(i, j)] == 1: 
                tmp += 1
                
        for j in range(1, N+1): 
            if ((j, i) in res) and i != j and res[(j, i)] == 1: 
                tmp += 1
        if tmp == 1: 
            res[(i, 0)] = 1.0

    for r in res:
        p = r[0]
        k = r[1]
        plt.plot([xcoord[p], xcoord[k]], [ycoord[p], ycoord[k]],alpha=0.4)
    plt.show()
            
if __name__ == "__main__":
    main()