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
#include <queue>

/**
 * @brief 
 * 
 * @param instance 
 * @return DTOPSolution 
 */
DTOPSolution SolomonRegretAlgorithm::Solve(const DTOPInstance& instance) const {
  std::vector<bool> visited(instance.GetNumberOfNodes(), false);
  std::vector<int> next_node_;
  std::vector<int> prev_node_;

  std::vector<int> first_node_(instance.GetNumberOfVehicles());  // First node of each route
  std::vector<int> last_node_(instance.GetNumberOfVehicles());   // Last node of each route


  // Para cada camión 
  for (int route{0}; route < instance.GetNumberOfVehicles(); ++route) {
    // Elegir cliente inicial de la ruta (por ejemplo el cliente más lejano del almacén)
    int initial_client = SelectInitialNode(instance.GetDistances()[0], visited);
    first_node_[route] = initial_client;
    // Para cada cliente que no tenga ruta asignada (En un futuro usar parametro q para no probar todos)
      // Obtener cuál sería su mejor lugar para insertar en la ruta actual
    // Insertar el cliente que tiene mejor desempeño -> Utilizar el arrepentimiento.
    // Si no hay más clientes con inserción factible -> Parar la ruta actual y continuar con la siguiente.
  }
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
 * @param q 
 * @return std::vector<int> 
 */
std::vector<int> GetNearestNeighbors(const std::vector<double>& distances, const std::vector<bool>& visited, int q) {
  std::priority_queue<std::pair<double, int>> nearest_neighbors_queue;

  for (int client_id{1}; client_id < distances.size(); ++client_id) {
    if (!visited[client_id]) {
      nearest_neighbors_queue.push({distances[client_id], client_id});
      if (nearest_neighbors_queue.size() > q) nearest_neighbors_queue.pop();
    }
  }

  std::vector<int> nearest_neighbors(q);
  while (!nearest_neighbors_queue.empty()) {
    // Sacar de la cola y meter en el vector
  }
  return nearest_neighbors;
}