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

  std::vector<int> first_node(instance.GetNumberOfVehicles());  // First node of each route
  std::vector<int> last_node(instance.GetNumberOfVehicles());   // Last node of each route

  int q = 4;
  // Para cada camión 
  for (int route{0}; route < instance.GetNumberOfVehicles(); ++route) {
    // Elegir cliente inicial de la ruta (por ejemplo el cliente más lejano del almacén)
    int current_reference_client = SelectInitialNode(instance.GetDistances()[0], visited);
    first_node[route] = current_reference_client;
    int route_accumulated_time = 0;
    visited[0] = true;
    while (true) {
      // Para cada cliente que no tenga ruta asignada (En un futuro usar parametro q para no probar todos)
      int selected_client{-1};
      double best_client_value{-1};
      std::vector<int> nearest_neighbors = GetNearestNeighbors(instance.GetDistances()[current_reference_client], visited, q); 

      int aux = first_node[route];
      std::vector<InsertionData> insertion_cost_data(q); 
      // Para cada hueco de la ruta
      while (aux != -1) {
        // Para cada candidato
        for (int i{0}; i < nearest_neighbors.size(); ++i) {
          const Client& succesor = next_node[aux] == -1 ? instance.GetClients()[0] : instance.GetClients()[next_node[aux]];
          // Calcular el coste de inserción
          double insertion_cost = CalculateInsertionCost(
            instance.GetClients()[aux], succesor, instance.GetClients()[nearest_neighbors[i]],
            route_accumulated_time, instance.GetMaxTime());
          // Almacenar el mejor y el segundo mejor coste de inserción
          if (insertion_cost < insertion_cost_data[i].best_insertion_cost) {
            insertion_cost_data[i].second_best_insertion_cost = insertion_cost_data[i].best_insertion_cost;
            insertion_cost_data[i].best_insertion_cost = insertion_cost;
            insertion_cost_data[i].predecessor = aux;
            insertion_cost_data[i].id = nearest_neighbors[i];
          } else if (insertion_cost < insertion_cost_data[i].second_best_insertion_cost) {
            insertion_cost_data[i].second_best_insertion_cost = insertion_cost;
            insertion_cost_data[i].id = nearest_neighbors[i];
          }
        }
        aux = next_node[aux];      
      }
      // Calcular el arrepentimiento de cada candidato.
      double best_regret = -1;
      double best_candidate = -1;
      for (int i{0}; i < q; ++i) {
        double candidate_regret = insertion_cost_data[i].second_best_insertion_cost - insertion_cost_data[i].best_insertion_cost; 
        if (candidate_regret > best_regret) {
          best_regret = candidate_regret;
          best_candidate = i;
        }
      }

      if (best_candidate == -1) break;

      // Añadir a la ruta el cliente que tenga el mayor arrepentimiento
      next_node[insertion_cost_data[best_candidate].id] = next_node[insertion_cost_data[best_candidate].predecessor];
      next_node[insertion_cost_data[best_candidate].predecessor] = insertion_cost_data[best_candidate].id;
      route_accumulated_time += insertion_cost_data[best_candidate].best_insertion_cost;
      visited[insertion_cost_data[best_candidate].id] = true;
      // Si no hay más clientes con inserción factible -> Parar la ruta actual y continuar con la siguiente.
      current_reference_client = insertion_cost_data[best_candidate].id;
      aux = current_reference_client;
    }
  }

  return DTOPSolution({
    next_node, prev_node, {}, {}, visited, {}
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

  for (int client_id{2}; client_id < depot_distances.size(); ++client_id) {
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

  for (int client_id{1}; client_id < distances.size(); ++client_id) {
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