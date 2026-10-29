/**
 * @file dtop_solution_builder.cc
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief 
 * @version 1.0
 * @date 29-10-2026
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../include/dtop_solution_builder.h"

DTOPSolutionBuilder::DTOPSolutionBuilder(const DTOPInstance& instance) {
  int num_nodes = instance.GetNumberOfNodes();
  int num_vehicles = instance.GetNumberOfVehicles();
  
  solution_.next_node.assign(num_nodes, -1);
  solution_.prev_node.assign(num_nodes, -1);

  solution_.first_node.assign(num_vehicles, -1);
  solution_.last_node.assign(num_vehicles, -1);
  solution_.route_profit.assign(num_vehicles, 0.0);
  solution_.route_duration.assign(num_vehicles, 0.0);
  solution_.visited.assign(num_nodes, false);
  // Falta inicializar los arrival times
}

void DTOPSolutionBuilder::InitializeRouteWithSeed(int route, int seed_id, double initial_cost, double reward) {
  solution_.next_node[seed_id] = -1;
  solution_.prev_node[seed_id] = -1;
  solution_.first_node[route] = seed_id;
  solution_.last_node[route] = seed_id;

  solution_.visited[seed_id] = true;
  solution_.route_duration[route] += initial_cost;
  solution_.route_profit[route] += reward;
}

void DTOPSolutionBuilder::InsertClient(int route, int candidate, int predecessor, double cost, double reward) {
  if (predecessor == 0) {  // The client is inserted at the beginning of the route (after the beginning depot)
    solution_.prev_node[candidate] = -1;
    solution_.next_node[candidate] = solution_.first_node[route];
    solution_.prev_node[solution_.first_node[route]] = candidate;
    solution_.first_node[route] = candidate;
  } else {
    solution_.next_node[candidate] = solution_.next_node[predecessor];
    solution_.prev_node[candidate] = predecessor;
    solution_.next_node[predecessor] = candidate;
  }

  if (solution_.next_node[candidate] == -1) {  // It is inserted at the end of the route (before the last depot)
    solution_.last_node[route] = candidate;
  } else {
    solution_.prev_node[solution_.next_node[candidate]] = candidate;
  }

  solution_.visited[candidate] = true;
  solution_.route_duration[route] += cost;
  solution_.route_profit[route] += reward;  
}

