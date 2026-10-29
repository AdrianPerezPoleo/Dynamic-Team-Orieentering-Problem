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
#include <algorithm>
#include <cmath>
#include <queue>

struct InsertionData {
  int id = -1;
  double best_insertion_cost = 1e6;
  double second_best_insertion_cost = 1e6;
  int predecessor;
};

/**
 * @brief 
 * 
 * @param instance 
 * @return DTOPSolution 
 */
DTOPSolution SolomonRegretAlgorithm::Solve(const DTOPInstance& instance) const {
  std::vector<bool> visited(instance.GetNumberOfNodes(), false);
  std::vector<int> next_node(instance.GetNumberOfNodes(), -1);
  std::vector<int> prev_node(instance.GetNumberOfNodes(), -1);

  std::vector<double> route_profit(instance.GetNumberOfVehicles());

  std::vector<int> first_node(instance.GetNumberOfVehicles());  // First node of each route
  std::vector<int> last_node(instance.GetNumberOfVehicles());   // Last node of each route

  std::vector<double> route_durations(instance.GetNumberOfVehicles());

  int q = 4;
  int end_depot = instance.GetNumberOfNodes() - 1;

  // Para cada camión 
  for (int route{0}; route < instance.GetNumberOfVehicles(); ++route) {
    // Elegir cliente inicial de la ruta (por ejemplo el cliente más lejano del almacén)
    int current_reference_client = SelectInitialNode(instance.GetDistances()[0], visited);
    if (current_reference_client == -1) break;

    first_node[route] = current_reference_client;
    last_node[route] = current_reference_client;
    next_node[current_reference_client] = -1;
    prev_node[current_reference_client] = -1;

    double route_accumulated_time = instance.GetDistances()[0][current_reference_client] + instance.GetDistances()[current_reference_client][instance.GetNumberOfNodes() - 1];
    route_profit[route] += instance.GetClients()[current_reference_client].reward;
    visited[current_reference_client] = true;

    while (true) {
      // Para cada cliente que no tenga ruta asignada (En un futuro usar parametro q para no probar todos)
      std::vector<int> nearest_neighbors = GetNearestNeighbors(instance.GetDistances()[current_reference_client], visited, q); 
      std::vector<InsertionData> insertion_cost_data(q); 

      int current = 0;

      // Para cada hueco de la ruta
      while (current != end_depot) {
        int succesor;
        if (current == 0) {
          succesor = first_node[route];
        } else {
          succesor = next_node[current] == -1 ? end_depot : next_node[current];
        }

        // Para cada candidato
        for (int i{0}; i < nearest_neighbors.size(); ++i) {
          const Client& predecessor = instance.GetClients()[current];
          const Client& successor = instance.GetClients()[succesor];
          const Client& candidate = instance.GetClients()[nearest_neighbors[i]]; 
          
          // Calcular el coste de inserción
          double insertion_cost = CalculateInsertionCost(
            predecessor, successor, candidate, route_accumulated_time, instance.GetMaxTime()
          );
          // Almacenar el mejor y el segundo mejor coste de inserción
          if (insertion_cost < insertion_cost_data[i].best_insertion_cost) {
            insertion_cost_data[i].second_best_insertion_cost = insertion_cost_data[i].best_insertion_cost;
            insertion_cost_data[i].best_insertion_cost = insertion_cost;
            insertion_cost_data[i].predecessor = current;
            insertion_cost_data[i].id = nearest_neighbors[i];
          } else if (insertion_cost < insertion_cost_data[i].second_best_insertion_cost) {
            insertion_cost_data[i].second_best_insertion_cost = insertion_cost;
            insertion_cost_data[i].id = nearest_neighbors[i];
          }
        }
        current = succesor;      
      }
      // Calcular el arrepentimiento de cada candidato.
      double best_regret = -1;
      int best_candidate = -1;
      for (int i{0}; i < nearest_neighbors.size(); ++i) {
        if (insertion_cost_data[i].id == -1) continue;  
        double candidate_regret = insertion_cost_data[i].second_best_insertion_cost - insertion_cost_data[i].best_insertion_cost; 
        if (candidate_regret > best_regret) {
          best_regret = candidate_regret;
          best_candidate = i;
        }
      }

      if (best_candidate == -1) break;
      int winner_id = insertion_cost_data[best_candidate].id;
      int predecessor = insertion_cost_data[best_candidate].predecessor;

      // Añadir a la ruta el cliente que tenga el mayor arrepentimiento
      if (predecessor == 0) {
        next_node[winner_id] = first_node[route];
        prev_node[winner_id] = -1;
        prev_node[first_node[route]] = winner_id;
        first_node[route] = winner_id;
      } else {
        next_node[winner_id] = next_node[predecessor];
        prev_node[winner_id] = predecessor;
        next_node[predecessor] = winner_id;

        if (next_node[winner_id] != -1) {
          prev_node[next_node[winner_id]] = winner_id;
        } else {
          last_node[route] = winner_id;
        }        
      }

      route_accumulated_time += insertion_cost_data[best_candidate].best_insertion_cost;
      visited[winner_id] = true;
      current_reference_client = winner_id;

      route_durations[route] = route_accumulated_time;
      route_profit[route] += instance.GetClients()[winner_id].reward;
    // Si no hay más clientes con inserción factible -> Parar la ruta actual y continuar con la siguiente.
    }
  }

  return DTOPSolution({
    next_node, prev_node, first_node, {}, route_profit, route_durations, visited, {}
  });
}

/**
 * @brief Selects the initial node of a route following the furthest point criterion.
 * 
 * @param depot_distances Distances of all clients to the depot.
 * @return int Id of thefurthest client.
 */
int SolomonRegretAlgorithm::SelectInitialNode(const std::vector<double>& depot_distances, const std::vector<bool>& visited) const {
  int max_distance_client = -1;
  double max_distance = depot_distances[0];

  for (int client_id{1}; client_id < depot_distances.size() - 1; ++client_id) {
    if (!visited[client_id] && depot_distances[client_id] > max_distance) {
      max_distance_client = client_id;
      max_distance = depot_distances[client_id];
    }
  }

  return max_distance_client;
}

/**
 * @brief Returns the q nearest neighbors given the distances of a client.
 * 
 * @param distances Distances of a client and the rest of clients.
 * @param q Number of neighbors to pick.
 * @return std::vector<int> 
 */
std::vector<int> SolomonRegretAlgorithm::GetNearestNeighbors(const std::vector<double>& distances, const std::vector<bool>& visited, int q) const {
  std::priority_queue<std::pair<double, int>> nearest_neighbors_queue;

  for (int client_id{1}; client_id < distances.size() - 1; ++client_id) {
    if (!visited[client_id]) {
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

double SolomonRegretAlgorithm::CalculateInsertionCost(const Client& predecessor, const Client& successor, const Client& candidate, 
                              double accumulated_time, double max_time) const {
  double removed_distance = std::hypot(predecessor.x_coordinate - successor.x_coordinate, predecessor.y_coordinate - successor.y_coordinate);
  double distance_to_candidate =   std::hypot(predecessor.x_coordinate - candidate.x_coordinate, predecessor.y_coordinate - candidate.y_coordinate);
  double distance_from_candidate = std::hypot(successor.x_coordinate - candidate.x_coordinate, successor.y_coordinate - candidate.y_coordinate);
  double insertion_cost = distance_to_candidate + distance_from_candidate - removed_distance;
  if (accumulated_time + insertion_cost < max_time) return insertion_cost;
  return 1e6;  // Devolver algo que descalifique automáticamente al nodo
}