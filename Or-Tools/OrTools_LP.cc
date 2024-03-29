#include <iostream>
#include <random>
#include <ctime>
#include "ortools/linear_solver/linear_solver.h"

namespace operations_research
{
    std::vector<std::vector<int>> printSubsequences(std::vector<int> &arr, int index,
                                                    std::vector<std::vector<int>> &subarr, int n, std::vector<int> &sub)
    {
        if (index == n)
        {
            if (sub.size() > 0)
            {
                subarr.push_back(sub);
            }
        }
        else
        {
            sub.push_back(arr[index]);

            printSubsequences(arr, index + 1, subarr, n, sub);

            sub.pop_back();

            printSubsequences(arr, index + 1, subarr, n, sub);
        }
        return subarr;
    }
    void LinearProgrammingExample(int N, int p, const std::vector<std::vector<double>> &dist_arr, std::string solver_name)
    {
        std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver(solver_name));
        if (!solver)
        {
            LOG(WARNING) << "SCIP solver unavailable.";
            return;
        }

        std::vector<std::vector<std::vector<const MPVariable*>>> x(p, std::vector<std::vector<const MPVariable*>>(N + 1, std::vector<const MPVariable*>(N + 1)));
        
        for (int k = 0; k < p; k++)
        {
            for (int i = 0; i < N + 1; i++)
            {
                for (int j = 0; j < N + 1; j++)
                {
                    x[k][i][j] = solver->MakeIntVar(0, 1, "");
                }
            }
        }

        // 2
        for (int j = 1; j < N + 1; j++)
        {
            LinearExpr constraint2_1;
            for (int k = 0; k < p; k++)
            {
                for (int i = 0; i < N + 1; i++)
                {
                    if (i != j)
                    {
                        constraint2_1 += x[k][i][j];
                    }
                }
            }
            solver->MakeRowConstraint(constraint2_1 == 1);
        }

        // 3
        for (int k = 0; k < p; k++)
        {
            LinearExpr constraint3;
            for (int j = 1; j < N + 1; j++)
            {
                constraint3 += x[k][0][j];
            }
            solver->MakeRowConstraint(constraint3 == 1);
        }

        // 4
        for (int k = 0; k < p; k++)
        {
            for (int j = 0; j < N + 1; j++)
            {
                LinearExpr constraint4_1, constraint4_2;
                for (int i = 0; i < N + 1; i++)
                {
                    if (i != j)
                    {
                        constraint4_1 += x[k][i][j];
                    }
                }

                for (int i = 0; i < N + 1; i++)
                {
                    if (i != j)
                    {
                        constraint4_2 += x[k][j][i];
                    }
                }
                solver->MakeRowConstraint(constraint4_1 == constraint4_2);
            }
        }

        // 6
        std::vector<int> arr;
        for (int i = 1; i < N + 1; i++)
        {
            arr.push_back(i);
        }

        std::vector<std::vector<int>> subs;
        std::vector<int> sub;
        int index = 0;

        subs = printSubsequences(arr, index, subs, N, sub);
        for (int S = 0; S < subs.size(); S++)
        {
            LinearExpr constraint6;
            for (int k = 0; k < p; k++)
            {
                for (int i : subs[S])
                {
                    for (int j : subs[S])
                    {
                        if (i != j)
                        {
                            constraint6 += x[k][i][j];
                        }
                    }
                }
            }
            solver->MakeRowConstraint(constraint6 <= (subs[S].size() - 1));
        }

        // Minimize function
        MPObjective *const objective = solver->MutableObjective();
        for (int k = 0; k < p; k++)
        {
            for (int i = 0; i < N + 1; i++)
            {
                for (int j = 0; j < N + 1; j++)
                {
                    if (i != j)
                    {
                        objective->SetCoefficient(x[k][i][j], dist_arr[i][j]);
                    }
                }
            }
        }
        objective->SetMinimization();

        std::cout << "I'm HEAR 4" << std::endl;
        const MPSolver::ResultStatus result_status = solver->Solve();
        if (result_status != MPSolver::OPTIMAL)
        {
            LOG(FATAL) << "The problem does not have an optimal solution.";
        }
        LOG(INFO) << "Solution:";
        LOG(INFO) << "Optimal objective value = " << objective->Value();

        for (int k = 0; k < p; k++)
        {
            for (int i = 0; i < N + 1; i++)
            {
                for (int j = 0; j < N + 1; j++)
                {
                    if (x[k][i][j]->solution_value() > 0.9)
                    {
                        LOG(INFO) << "x[" << k << "]"
                                  << "[" << i << "]"
                                  << "[" << j << "] = " << x[k][i][j]->solution_value();
                    }
                }
            }
        }
    }
} // namespace operations_research

int main(int argc, char **argv)
{
    int N = 20;
    int p = 6;
    std::string solver_name = "SCIP";
    std::vector<std::vector<double>> dist_arr(N + 1, std::vector<double>(N + 1, 0));

    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_real_distribution<double> distr(1, 100);

    for (int j = 0; j < N + 1; j++)
    {
        for (int i = 0; i < N + 1; i++)
        {
            if (i != j)
            {
                dist_arr[i][j] = distr(generator);
                dist_arr[j][i] = dist_arr[i][j];
            }
        }
    }
    std::cout << "I'm READY" << std::endl;
    unsigned int start_time = clock();
    operations_research::LinearProgrammingExample(N, p, dist_arr, solver_name);
    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    std::cout << "Время работы: " << search_time / 1000000.0 << "секунд" << std::endl;
    return EXIT_SUCCESS;
}
