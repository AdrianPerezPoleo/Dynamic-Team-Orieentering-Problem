/**
 * @file dtop_solution_builder.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief 
 * @version 1.0
 * @date 29-10-2026
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef DTOP_SOLUTION_BUILDER_h
#define DTOP_SOLUTION_BUILDER_h

#include "dtop_instance.h"
#include "dtop_solution.h"

class DTOPSolutionBuilder {
 public:
  DTOPSolutionBuilder(const DTOPInstance& instance);

  double GetRouteTime(int route) const { return solution_.route_duration[route]; }
  bool IsVisited(int client_id) const { return solution_.visited[client_id]; }
  int GetFirstNode(int route) const { return solution_.first_node[route]; }
  int GetNextNode(int node) const { return solution_.next_node[node]; }

  void InsertClient(int route, int candidate, int predecessor, double cost, double reward);
  void InitializeRouteWithSeed(int route, int seed_id, double initial_cost, double reward);

  DTOPSolution Build() const { return DTOPSolution(std::move(solution_)); }
 private:
  DTOPSolutionData solution_;
};


#endif