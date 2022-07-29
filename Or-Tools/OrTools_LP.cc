#include <iostream>
#include <random>
#include <ctime>
#include "ortools/linear_solver/linear_solver.h"

namespace operations_research
{
  void LinearProgrammingExample(int N, int p, const std::vector<std::vector<int>> &dist_arr ,std::string solver_name)
  {
    std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver(solver_name));
    if (!solver)
    {
      LOG(WARNING) << "SCIP solver unavailable.";
      return;
    }

    // x and y are non-negative variables.
    std::vector<std::vector<std::vector<const MPVariable*>>> x(p, std::vector<std::vector<const MPVariable*>>(N+1, std::vector<const MPVariable*>(N+1, 0)));
    for (int k = 0; k < p; k++)
    {
      for (int i = 0; i < N+1; i++)
      {
        for (int j = 0; j < N+1; j++)
        {
          x[k][i][j] = solver->MakeIntVar(0.0, 1.0, "");
        }
      }
    }
    std::cout<< "I'm HEAR 1" << std::endl;

    //2.1
    for(int j = 1; j < N+1; j++)
    {
      LinearExpr constraint2_1;
      for (int k = 0; k < p; k++)
      {
        for (int i = 0; i < N+1; i++)
        {
          if(i != j)
          {
            constraint2_1 += x[k][i][j];
          }
        }
      }
      solver->MakeRowConstraint(constraint2_1 == 1);
    }

    //2.2
    for(int i = 1; i < N+1; i++)
    {
      LinearExpr constraint2_2;
      for (int k = 0; k < p; k++)
      {
        for (int j = 0; j < N+1; j++)
        {
          if(i != j)
          {
            constraint2_2 += x[k][i][j];
          }
        }
      }
      solver->MakeRowConstraint(constraint2_2 == 1);
    }
    std::cout<< "I'm HEAR 2" << std::endl;

    //3
    for(int k = 0; k < p; k++)
    {
      LinearExpr constraint3;
      for(int j = 1; j < N+1; j++)
      {
        constraint3 += x[k][0][j];
      }
      solver->MakeRowConstraint(constraint3 == 1);
    }
    std::cout<< "I'm HEAR 3" << std::endl;

    //4
    const double infinity = solver->infinity();
    for(int k = 0; k < p; k++)
    {
      for(int j = 0; j < N+1; j++)
      {
        LinearExpr constraint4_1, constraint4_2;
        for(int i = 0; i < N+1; i++)
        {
          if(i != j)
          {
            constraint4_1 += x[k][i][j];
          }
        }

        for(int i = 0; i < N+1; i++)
        {
          if(i != j)
          {
            constraint4_2 += x[k][j][i];
          }
        }
        solver->MakeRowConstraint(constraint4_1 == constraint4_2);
      }
    }

    //6
    std::vector<int> arr;
    for(int i = 1; i < N+1; i++)
    {
        arr.push_back(i);
    }

    std::vector<std::vector<int>> subs;
    int start = 0, end = 0;
    while(1)
    {
        if (end == arr.size())
        {
            break;
        }
        else if (start > end)
        {
            start = 0;
            end++;
        }
        else 
        {
            std::vector<int> sub = {};
            for (int i = start; i < end; i++)
            {
                sub.push_back(arr[i]);
            }
            if(sub.size() > 0)
            {
                subs.push_back(sub);
                sub.clear();
            }
            start++;
        }
    }

    for(int S = 0; S < subs.size(); S++)
    {
      LinearExpr constraint6;
      for(int k = 0; k < p; k++)
      {
        for(int i : subs[S])
        {
          for(int j : subs[S])
          {
            if(i != j)
            {
              constraint6 += x[k][i][j];
            }
          }
        }
      }
      solver->MakeRowConstraint(constraint6 <= subs[S].size() - 1);
    }

    // Minimize function
    MPObjective* const objective = solver->MutableObjective();
    for(int k = 0; k < p; k++)
    {
      for(int i = 0; i < N+1; i++)
      {
        for(int j = 0; j < N+1; j++)
        {
          if(i != j)
          {
            objective->SetCoefficient(x[k][i][j], dist_arr[i][j]);
          }
        }
      }
    }
    objective->SetMinimization();

    std::cout<< "I'm HEAR 4" << std::endl;
    const MPSolver::ResultStatus result_status = solver->Solve();
    if (result_status != MPSolver::OPTIMAL) 
    {
      LOG(FATAL) << "The problem does not have an optimal solution.";
    }
    LOG(INFO) << "Solution:";
    LOG(INFO) << "Optimal objective value = " << objective->Value();

    for(int k = 0; k < p; k++)
    {
      for(int i = 0; i < N+1; i++)
      {
        for(int j = 0; j < N+1; j++)
        {
          if(x[k][i][j]->solution_value() > 0.9)
          {
            LOG(INFO) << "x[" << k  << "]" << "[" << i  << "]" << "[" << j  << "] = " << x[k][i][j]->solution_value();
          }
        }
      }
    }
    std::cout<< "I'm HEAR 5" << std::endl;
  }
} // namespace operations_research

int main(int argc, char **argv)
{
  int N = 20;
  int p = 5;
  std::string solver_name = "SCIP";
  std::vector<std::vector<int>> dist_arr(N+1, std::vector<int>(N+1, 0));

  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_real_distribution<double> distr(1, 100);

  for (int j = 0; j < N+1; j++)
  {
      for (int i = 0; i < N+1; i++)
      {
          if(i != j)
          {
              dist_arr[i][j] = distr(generator);
              dist_arr[j][i] = dist_arr[i][j];
          }
      }
  }
  std::cout<< "I'm READY" << std::endl;
  unsigned int start_time =  clock();
  operations_research::LinearProgrammingExample(N, p, dist_arr, solver_name);
  unsigned int end_time = clock();
  unsigned int search_time = end_time - start_time;
  std::cout << "Время работы: " << search_time/1000000.0 << "секунд" << std::endl;
  return EXIT_SUCCESS;
}
