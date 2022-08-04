#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>
#include "ortools/linear_solver/linear_solver.h"

namespace operations_research
{
    void LinearProgrammingExample(int N, int p, int Q, const std::vector<int> &d, 
    const std::vector<std::vector<double>> &new_matrix, std::string solver_name, std::vector<int> &del_point)
    {
        std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver(solver_name));
        if (!solver)
        {
            LOG(WARNING) << "SCIP solver unavailable.";
            return;
        }

        std::vector<std::vector<const MPVariable*>> x(N + 1, std::vector<const MPVariable*>(N + 1, 0));
        std::vector<const MPVariable*> y(N + 1, 0);

        int count1 = 0;
        for (int i = 0; i < N + 1; i++)
        {
            for (int j = 0; j < N + 1; j++)
            {
                x[i][j] = solver->MakeIntVar(0, 1, "");
                count1++;    
            }
        }

        int count2 = 0;
        for (int i = 1; i < N+1; i++)
        {
            y[i] = solver->MakeIntVar(1, 42, "");
            count2++;
        }

        // 9
        int count3 = 0;
        LinearExpr constraint9;
        for (int j = 1; j < N + 1; j++)
        {
            if(std::find(del_point.begin(), del_point.end(), j) == del_point.end())
            {
                constraint9 += x[0][j];
                count3++;
            }
        }
        solver->MakeRowConstraint(constraint9 == p);

        // 10
        int count4 = 0;
        for(int j = 1; j < N+1; j++)
        {
            if(std::find(del_point.begin(), del_point.end(), j) == del_point.end())
            {
                LinearExpr constraint10;
                for(int i = 0; i < N+1; i++)
                {
                    if(i != j && std::find(del_point.begin(), del_point.end(), i) == del_point.end())
                    {
                        constraint10 += x[i][j];
                        count4++;
                    }
                }
                solver->MakeRowConstraint(constraint10 == 1);
            }
        }

        // 11
        int count5 = 0;
        for(int i = 1; i < N+1; i++)
        {
            if(std::find(del_point.begin(), del_point.end(), i) == del_point.end())
            {
                LinearExpr constraint11;
                for(int j = 1; j < N+1; j++)
                {
                    if(i != j && std::find(del_point.begin(), del_point.end(), j) == del_point.end())
                    {
                        constraint11 += x[i][j];
                        count5++;
                    }
                }
                solver->MakeRowConstraint(constraint11 <= 1);
            }
        }

        // 12
        int count6 = 0;
        const double infinity = solver->infinity();
        for(int i = 1; i < N+1; i++)
        {
            if(std::find(del_point.begin(), del_point.end(), i) == del_point.end())
            {
                for(int j = 1; j < N+1; j++)
                {
                    if(i != j && std::find(del_point.begin(), del_point.end(), j) == del_point.end())
                    {
                        int sum_demand = d[j] + Q;
                        // -inf <= y[i] * 1 + y[j] * (-1) + x[i][j] * d[j] + x[i][j] * Q <= Q
                        MPConstraint* constraint12 = solver->MakeRowConstraint(-infinity, Q, "");
                        constraint12->SetCoefficient(y[i], 1);
                        constraint12->SetCoefficient(y[j], -1);
                        constraint12->SetCoefficient(x[i][j], sum_demand);
                        count6++;
                    }
                }
            }
        }

        // 13
        int count7 = 0;
        for(int i = 1; i < N+1; i++)
        {
            if(std::find(del_point.begin(), del_point.end(), i) == del_point.end())
            {
                MPConstraint* constraint13 = solver->MakeRowConstraint(d[i], Q, "");
                constraint13->SetCoefficient(y[i], 1);
                count7++;
            }
        }

        // Maximize function
        int count8 = 0;
        MPObjective *const objective = solver->MutableObjective();
        for (int i = 1; i < N + 1; i++)
        {
            if(std::find(del_point.begin(), del_point.end(), i) == del_point.end())
            {
                for (int j = 1; j < N + 1; j++)
                {
                    if (i != j && std::find(del_point.begin(), del_point.end(), j) == del_point.end())
                    {
                        objective->SetCoefficient(x[i][j], new_matrix[i][j]);
                        count8++;
                    }
                }
            }
        }
        objective->SetMaximization();

        // std::cout << "СЧЁТЧИК1: " << count1 << std::endl;
        // std::cout << "СЧЁТЧИК2: " << count2 << std::endl;
        // std::cout << "СЧЁТЧИК3: " << count3 << std::endl;
        // std::cout << "СЧЁТЧИК4: " << count4 << std::endl;
        // std::cout << "СЧЁТЧИК5: " << count5 << std::endl;
        // std::cout << "СЧЁТЧИК6: " << count6 << std::endl;
        // std::cout << "СЧЁТЧИК7: " << count7 << std::endl;
        // std::cout << "СЧЁТЧИК8: " << count8 << std::endl;
        const MPSolver::ResultStatus result_status = solver->Solve();
        if (result_status != MPSolver::OPTIMAL)
        {
            LOG(FATAL) << "The problem does not have an optimal solution.";
        }
        LOG(INFO) << "Solution:";
        LOG(INFO) << "Optimal objective value = " << objective->Value();
        for (int i = 0; i < N + 1; i++)
        {
            if(std::find(del_point.begin(), del_point.end(), i) == del_point.end())
            {
                for (int j = 0; j < N + 1; j++)
                {
                    if (x[i][j]->solution_value() > 0.9 && std::find(del_point.begin(), del_point.end(), j) == del_point.end())
                    {
                        LOG(INFO) << "x[" << i << "]"
                                << "["  << j << "] = " << x[i][j]->solution_value();
                    }
                }
            }
        }
    }
} // namespace operations_research

// 
// 10: -10   max_capacity = 20
// 20: -20   max_capacity = 30
// 50: -50   max_capacity = 40
// 100: -100 max_capacity = 50
// 250: -250
// 1000: -1000
// 2500: -2500
int main(int argc, char **argv)
{
    int N = 10;
    int p = 5;
    int max_capacity = 20;
    std::string solver_name = "SCIP";
    std::random_device device;
    std::mt19937 generator(device());
    
    std::uniform_real_distribution<double> cap(1, 42);
    std::vector<int> capacity(N+1, 0);
    std::vector<int> del_point;
    for(int i = 1; i < N+1; i++)
    {
        capacity[i] = cap(generator);
        if(capacity[i] > max_capacity)
        {
            capacity[i] = 0;
            del_point.push_back(i);
        }
    }
    
    std::vector<std::vector<double>> dist_arr(N + 1, std::vector<double>(N + 1, 0));
    std::uniform_real_distribution<double> distr(1, 100);
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (i != j)
            {
                dist_arr[i][j] = distr(generator);
                dist_arr[j][i] = dist_arr[i][j];
            }
        }
    }
    
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            for(int k = 0; k < N + 1; k++)
            {
                if(i != j && i != k && j != k)
                {
                    while(1)
                    {
                        if(dist_arr[i][j] + dist_arr[j][k] >= dist_arr[i][k])
                        {
                            break;
                        }
                        else
                        {
                            dist_arr[i][j] = distr(generator);
                            dist_arr[j][i] = dist_arr[i][j];
                        }
                    }
                }
            }
        }
    }

    std::vector<std::vector<double>> new_matrix(N + 1, std::vector<double>(N + 1, 0));
    for (int i = 1; i < N + 1; i++)
    {
        if(std::find(del_point.begin(), del_point.end(), i) == del_point.end())
        {
            for (int j = 1; j < N + 1; j++)
            {
                if (i != j && std::find(del_point.begin(), del_point.end(), j) == del_point.end())
                {
                    new_matrix[i][j] = dist_arr[i][0] + dist_arr[0][j] - dist_arr[i][j];
                    new_matrix[j][i] = new_matrix[i][j];
                }
            }
        }
    }

    std::cout << "I'm READY" << std::endl;
    unsigned int start_time = clock();
    operations_research::LinearProgrammingExample(N, p, max_capacity, capacity, new_matrix, solver_name, del_point);
    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    std::cout << "Время работы: " << search_time / 1000000.0 << "секунд" << std::endl;
    for (int i = 0; i < N + 1; i++)
    {
        std::cout << capacity[i] << " ";
    }
    return EXIT_SUCCESS;
}
