/**
 * @file dtop_solution.cc
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Implementation of the methods of the DTOP solution class.
 * @version 1.0
 * @date 2026-10-31
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../include/dtop_solution.h"

/**
 * @brief Calculates the total profit collected across all routes in the current solution.
 * 
 * @return double The sum of the rewards from all clients visited by all vehicles.
 */
double DTOPSolution::GetTotalProfit() const {
  double total_profit{0.0};
  for (const auto& route : route_profit_) {
    total_profit += route;
  }
  return total_profit;
}

/**
 * @brief Physically inserts a client into a specific route and updates the internal topology.
 * 
 * @param route The index of the route where the client will be inserted.
 * @param candidate The ID of the client to insert.
 * @param predecessor The ID of the node that will immediately precede the inserted client (0 for depot).
 * @param cost The calculated time increment (delta) added to the route's total duration.
 * @param reward The profit/reward provided by the inserted client.
 */
void DTOPSolution::InsertClient(int route, int candidate, int predecessor, double cost, double reward) {
  if (predecessor == 0) {  // The client is inserted at the beginning of the route (after the beginning depot)
    prev_node_[candidate] = -1;
    next_node_[candidate] = first_node_[route];
    prev_node_[first_node_[route]] = candidate;
    first_node_[route] = candidate;
  } else {
    next_node_[candidate] = next_node_[predecessor];
    prev_node_[candidate] = predecessor;
    next_node_[predecessor] = candidate;
  }

  if (next_node_[candidate] == -1) {  // It is inserted at the end of the route (before the last depot)
    last_node_[route] = candidate;
  } else {
    prev_node_[next_node_[candidate]] = candidate;
  }

  visited_[candidate] = true;
  route_duration_[route] += cost;
  route_profit_[route] += reward;  
}

/**
 * @brief Extracts a client from its current route and restores the topological links.
 * 
 * @param route The index of the route containing the client.
 * @param client The ID of the client to be removed.
 * @param time_saved The absolute time recovered by removing the client (delta).
 * @param reward_lost The profit/reward to be subtracted from the route's total profit.
 */
void DTOPSolution::RemoveClient(int route, int client, double time_saved, double reward_lost) {
  int predecessor = prev_node_[client];
  int successor = next_node_[client];

  if (predecessor == -1) {  // The client is inserted at the beginning of the route (after the beginning depot)
    first_node_[route] = successor;
  } else {
    next_node_[predecessor] = successor;
  }

  if (successor== -1) {  // It is inserted at the end of the route (before the last depot)
    last_node_[route] = predecessor;
  } else {
    prev_node_[successor] = predecessor;
  }

  prev_node_[client] = -1;
  next_node_[client] = -1;

  visited_[client] = false;
  route_duration_[route] -= time_saved;
  route_profit_[route] -= reward_lost; 
}

/**
 * @brief Computes the delta of inserting a candidate node.
 * 
 * @param predecessor The ID of the node immediately before the proposed insertion point.
 * @param successor The ID of the node immediately after the proposed insertion point.
 * @param candidate The ID of the client being evaluated for insertion.
 * @param distances The precomputed distance matrix of the problem instance.
 * @return double The net time increment caused by the insertion.
 */
double DTOPSolution::CalculateInsertionCost(int predecessor, int successor, int candidate, const std::vector<std::vector<double>>& distances) const {
  double removed_distance = distances[predecessor][successor];
  double distance_to_candidate = distances[predecessor][candidate];
  double distance_from_candidate = distances[candidate][successor];
  double insertion_cost = distance_to_candidate + distance_from_candidate - removed_distance;
  return insertion_cost;
}

/**
 * @brief Computes the delta of removing an existing client.
 * 
 * @param predecessor The ID of the node preceding the client to remove.
 * @param client The ID of the client being evaluated for removal.
 * @param successor The ID of the node succeeding the client to remove.
 * @param distances The precomputed distance matrix of the problem instance.
 * @return double The net time variation resulting from the bypass.
 */
double DTOPSolution::CalculateRemovalCost(int predecessor, int client, int successor, const std::vector<std::vector<double>>& distances) const {
  double new_distance = distances[predecessor][successor];
  double distance_to_removed = distances[predecessor][client];
  double distance_from_removed = distances[client][successor];
  double insertion_cost = new_distance - (distance_from_removed + distance_to_removed);
  return std::abs(insertion_cost);
}