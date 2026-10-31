/**
 * @file solomon_algorithm.cc
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Implementation of the Solomon I1 heuristic for the VRSPTW with regret.
 * @version 1.0
 * @date 27-10-2026
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../../include/algorithm/solomon_regret_algorithm.h"
#include "../../include/dtop_solution_builder.h"
#include "../../include/random_generator.h"

#include <algorithm>
#include <cmath>
#include <queue>

/**
 * @brief Executes the Solomon Regret construction heuristic to solve the DTOP.
 * 
 * @param instance The DTOP instance to be solved.
 * @return DTOPSolution The final constructed solution built by the heuristic.
 */
DTOPSolution SolomonRegretAlgorithm::Solve(const DTOPInstance& instance) const {
  DTOPSolutionBuilder solution(instance);
  int q = 4;
  int k = 4;
  int end_depot = instance.GetNumberOfNodes() - 1;

  // Para cada camión 
  for (int route{0}; route < instance.GetNumberOfVehicles(); ++route) {
    // Elegir cliente inicial de la ruta (por ejemplo el cliente más lejano del almacén)
    int seed_id = SelectInitialNode(instance.GetDistances()[0], solution, k);
    if (seed_id == -1) break;
      
    double initial_time = instance.GetDistances()[0][seed_id] + instance.GetDistances()[seed_id][end_depot];
    int current_reference_client = seed_id;

    solution.InitializeRouteWithSeed(route, seed_id, initial_time, instance.GetClients().at(seed_id).reward);

    while (true) {
      // Para cada cliente que no tenga ruta asignada
      std::vector<int> nearest_neighbors = GetNearestNeighbors(instance.GetDistances()[current_reference_client], solution, q); 
      // Si no hay más clientes con inserción factible -> Parar la ruta actual y continuar con la siguiente.    
      if (nearest_neighbors.empty()) break;
      std::vector<InsertionData> insertion_cost_data(nearest_neighbors.size()); 
      EvaluateInsertions(route, nearest_neighbors, solution, instance, insertion_cost_data);

      // Calcular el arrepentimiento de cada candidato.
      int best_candidate = SelectBestCandidateByRegret(insertion_cost_data);
      if (best_candidate == -1) break;

      int winner_id = insertion_cost_data[best_candidate].id;
      double winner_cost = insertion_cost_data[best_candidate].best_insertion_cost;
      int predecessor = insertion_cost_data[best_candidate].predecessor;

      // Añadir a la ruta el cliente que tenga el mayor arrepentimiento
      solution.InsertClient(route, winner_id, predecessor, winner_cost, instance.GetClients()[winner_id].reward);
      current_reference_client = winner_id;
    }
  }
  return solution.Build();
}

/**
 * @brief Selects the initial node of a route following the furthest point criterion.
 * 
 * @param depot_distances Distances of all clients to the depot.
 * @return int Id of thefurthest client.
 */
int SolomonRegretAlgorithm::SelectInitialNode(const std::vector<double>& depot_distances, const DTOPSolutionBuilder& solution, int k, int seed) const {
  std::vector<std::pair<double, int>> distances;
  for (int i{1}; i < depot_distances.size() - 1; ++i) {
    if (!solution.IsVisited(i)) {
      distances.emplace_back(depot_distances[i], i);
    }
  }
  std::sort(distances.begin(), distances.end());

  std::vector<std::pair<double, int>> candidates_list;
  for (int client_id{1}; client_id < distances.size() - 1; ++client_id) {
    candidates_list.emplace_back(distances[client_id]);
    if (candidates_list.size() == k) break;
  }

  return candidates_list[generator_.GetInt(0, candidates_list.size() - 1)].second;
}

/**
 * @brief Returns the q nearest neighbors given the distances of a client.
 * 
 * @param distances Distances of a client and the rest of clients.
 * @param q Number of neighbors to pick.
 * @return std::vector<int> 
 */
std::vector<int> SolomonRegretAlgorithm::GetNearestNeighbors(const std::vector<double>& distances,const DTOPSolutionBuilder& solution, int q) const {
  std::priority_queue<std::pair<double, int>> nearest_neighbors_queue;

  for (int client_id{1}; client_id < distances.size() - 1; ++client_id) {
    if (!solution.IsVisited(client_id)) {
      nearest_neighbors_queue.push({distances[client_id], client_id});
      if (nearest_neighbors_queue.size() > q) nearest_neighbors_queue.pop();
    }
  }

  std::vector<int> nearest_neighbors;
  while (!nearest_neighbors_queue.empty()) {
    nearest_neighbors.emplace_back(nearest_neighbors_queue.top().second);
    nearest_neighbors_queue.pop();
  }
  std::reverse(nearest_neighbors.begin(), nearest_neighbors.end());
  return nearest_neighbors;
}

/**
 * @brief Calculates the net time cost of inserting a candidate client between a predecessor and a successor.
 * 
 * @param predecessor The ID of the node immediately before the proposed insertion point.
 * @param successor The ID of the node immediately after the proposed insertion point.
 * @param candidate The ID of the client being evaluated for insertion.
 * @param distances The precomputed distance matrix of the instance.
 * @param accumulated_time The current total duration of the route prior to insertion.
 * @param max_time The maximum allowed duration (T_max) for any route.
 * @return double The structural insertion cost if feasible, or a large penalty value (1e6) if it violates the time window.
 */
double SolomonRegretAlgorithm::CalculateInsertionCost(int predecessor, int successor, int candidate,
                              const std::vector<std::vector<double>>& distances, double accumulated_time, double max_time) const {
  double removed_distance = distances[predecessor][successor];
  double distance_to_candidate = distances[predecessor][candidate];
  double distance_from_candidate = distances[candidate][successor];
  double insertion_cost = distance_to_candidate + distance_from_candidate - removed_distance;
  if (accumulated_time + insertion_cost < max_time) return insertion_cost;
  return 1e6;  // Devolver algo que descalifique automáticamente al nodo
}

/**
 * @brief Evaluates all possible insertion positions in a route for a given set of candidate clients.
 * 
 * @param route The index of the route being evaluated.
 * @param candidates A vector containing the IDs of the candidate clients to evaluate.
 * @param solution The current state of the solution builder.
 * @param instance The DTOP instance containing problem constraints and parameters.
 * @param insertion_data An output vector populated with the best and second-best insertion costs found for each candidate.
 */
void SolomonRegretAlgorithm::EvaluateInsertions(int route, const std::vector<int>& candidates, 
                          const DTOPSolutionBuilder& solution, const DTOPInstance& instance, 
                          std::vector<InsertionData>& insertion_data) const {
  int end_depot = instance.GetNumberOfNodes() - 1;
  int current = 0;

  // Para cada hueco de la ruta
  while (current != end_depot) {
    int succesor;
    if (current == 0) {
      succesor = solution.GetFirstNode(route);
    } else {
      succesor = solution.GetNextNode(current) == -1 ? end_depot : solution.GetNextNode(current);
    }

    // Para cada candidato
    for (int i{0}; i < candidates.size(); ++i) {         
      // Calcular el coste de inserción
      double insertion_cost = CalculateInsertionCost(
        current, succesor, candidates[i], instance.GetDistances(), 
        solution.GetRouteTime(route), instance.GetMaxTime()
      );
      // Almacenar el mejor y el segundo mejor coste de inserción
      if (insertion_cost < insertion_data[i].best_insertion_cost) {
        insertion_data[i].second_best_insertion_cost = insertion_data[i].best_insertion_cost;
        insertion_data[i].best_insertion_cost = insertion_cost;
        insertion_data[i].predecessor = current;
        insertion_data[i].id = candidates[i];
      } else if (insertion_cost < insertion_data[i].second_best_insertion_cost) {
        insertion_data[i].second_best_insertion_cost = insertion_cost;
        insertion_data[i].id = candidates[i];
      }
    }
    current = succesor;      
  }
}

/**
 * @brief Selects the best candidate for insertion based on the maximum regret criterion.
 * 
 * @param insertion_data A vector containing the evaluated insertion costs for all considered candidates.
 * @return int The index (within the insertion_data vector) of the winning candidate, or -1 if no candidates are feasible.
 */
int SolomonRegretAlgorithm::SelectBestCandidateByRegret(const std::vector<InsertionData>& insertion_data) const {
  double best_regret = -1;
  int best_candidate = -1;
  for (int i{0}; i < insertion_data.size(); ++i) {
    if (insertion_data[i].id == -1) continue;  
    double candidate_regret = insertion_data[i].second_best_insertion_cost - insertion_data[i].best_insertion_cost; 
    if (candidate_regret > best_regret) {
      best_regret = candidate_regret;
      best_candidate = i;
    }
  }
  return best_candidate;
}