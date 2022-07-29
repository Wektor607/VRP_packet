import ortools
from ortools.linear_solver import pywraplp
import matplotlib.pyplot as plt
import numpy as np
from colour import Color
import random
# import mip_or_tools

class VRP_OrTools():
    def __init__(self, N, c, p, solver_name, clients):
        self.N = N                     # количество точек
        self.c = c                     # матрица расстояний
        self.p = p                     # количество транспортных средств
        self.solver_name = solver_name # название решателя
        self.clients = clients         # список клиентов
        
    def MIP(self):
        solver = pywraplp.Solver.CreateSolver(solver_name)
        x = [[[solver.IntVar(0, 1, 'x[%r][%i][%i]' % (r, j, i)) for i in range(self.N+1)] for j in range(self.N+1)] for r in range(self.p)]
        
        #Constraints
        #2
        for j in range(1, self.N+1):
            summa_2 = 0
            for r in range(0, self.p):
                for i in range(0, self.N+1):
                    if(i != j):
                        summa_2 += x[r][i][j]
            solver.Add(summa_2 == 1)

        #3
        for r in range(0, self.p):
            summa_3 = 0
            for j in range(1, self.N+1):
                summa_3 += x[r][0][j]
            solver.Add(summa_3 == 1)

        #4
        for r in range(0, self.p):
            for j in range(0, self.N+1):
                summa_4_1 = 0
                summa_4_2 = 0
                for i in range(0, self.N+1):
                    if(i != j):
                        summa_4_1 += x[r][i][j]
                
                for i in range(0, self.N+1):
                    if(i != j):
                        summa_4_2 += x[r][j][i]

                solver.Add(summa_4_1 == summa_4_2)

        #6
        s=set(range(1, self.N+1))
        L=list(s)
        subs = []
        for i in range(1,1<<len(L)):
            u = set()
            for j in range(len(L)):
                if 1<<j&i and len(u) <= self.N-3:
                    u.add(L[j])
            if(len(u) >= 3):
                subs.append(u)

        for S in subs:
            summa_6 = 0
            for r in range(self.p):
                for i in S:
                    for j in S:
                        if(i != j):
                            summa_6 += x[r][i][j]
            solver.Add(summa_6 <= len(S) - 1)
        
        # Minimize function
        summa = 0
        for r in range(0, self.p):
            for i in range(0, self.N+1):
                for j in range(0, self.N+1):
                    if(i != j):
                        summa = summa + self.c[(i, j)] * x[r][i][j]
        solver.Minimize(summa)
        
        status = solver.Solve()
        res = []
        if status == pywraplp.Solver.OPTIMAL:
            print('Objective value =', solver.Objective().Value())
            for r in range(self.p):
                for i in range(self.N+1):
                    for j in range(self.N+1):
                        if(x[r][i][j].solution_value() > 0.9):
                            res.append(((i,j), x[r][i][j].solution_value()))
                            print(x[r][i][j].name(), ' = ', x[r][i][j].solution_value())
            print()
            print('Problem solved in %f milliseconds' % solver.wall_time())
            print('Problem solved in %d iterations' % solver.iterations())
            print('Problem solved in %d branch-and-bound nodes' % solver.nodes())
        else:
            print('The problem does not have an optimal solution.')
        
        return res

def generate_data(n_customers, val, nodes):
    xcoord = [] 
    ycoord = []
    cities = rnds.uniform(size=(n_customers, 2))
    for i in range(len(cities)):
        xcoord.append(cities[i][0] * 100)
        ycoord.append(cities[i][1] * 100)
    dist_matrix = {(i, j): np.hypot(xcoord[i]-xcoord[j], ycoord[i]-ycoord[j]) for i in nodes for j in nodes if i != j}
    return dist_matrix, xcoord, ycoord

rnds = np.random
n_samples = 1
n_customers = 10
val = 100
solver_name = 'SCIP'#'GLOP'
count_vehicle = 5
i = 0
all_results = []
# while(i < n_samples):
clients = [i for i in range(n_customers+1) if i != 0]
nodes = [0] + clients
routes = [(i, j) for i in nodes for j in nodes if i != j]
dist_matrix, xcoord, ycoord = generate_data(n_customers+1, val, nodes)
print(dist_matrix)
res = VRP_OrTools(n_customers, dist_matrix, count_vehicle, solver_name, clients).MIP()

#Paint

plt.figure(figsize=(12,5))
plt.scatter(xcoord, ycoord, color="red")

plt.scatter(xcoord[0], ycoord[0], color="green", marker='D')
plt.annotate("Depo", (xcoord[0]-1, ycoord[0]-5.5))

for i in clients:
    plt.annotate('${%d}$' % i, (xcoord[i]-1, ycoord[i]-5))

for r in range(len(res)):
    p = res[r][0][0]
    k = res[r][0][1]
    plt.plot([xcoord[p], xcoord[k]], [ycoord[p], ycoord[k]],alpha=0.4)
plt.show()