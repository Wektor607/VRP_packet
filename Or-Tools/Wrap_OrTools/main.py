from collections import deque
from datetime import datetime
from functions import *

def main():
    N = 50
    p = 25
    maxCapacity = 100
    solver_name = 'SCIP'
    a = CVRP(N, p, maxCapacity, solver_name)
    print(a.LP_OrTools())
            
if __name__ == "__main__":
    main()