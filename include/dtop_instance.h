/**
 * @file dtop_instance.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Definition of a DTOP instance.
 * @version 1.0
 * @date 2026-10-24
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef DTOP_INSTANCE_H
#define DTOP_INSTANCE_H

#include <iostream>
#include <tuple>
#include <vector>

struct Client {
  int id;
  double x_coordinate;
  double y_coordinate;
  double reward;
  double arrival_time;

  bool operator<(const Client& other) const {
    return std::tie(arrival_time, other.reward, id) < 
           std::tie(other.arrival_time, reward, other.id);
  }
};

class DTOPInstance {
 public:
  DTOPInstance(int nodes, int vehicles, double max_time, const std::vector<Client>& clients) :
    number_of_nodes_{nodes}, number_of_vehicles_{vehicles}, max_time_{max_time}, clients_{clients} { }

  int GetNumberOfNodes() const { return number_of_nodes_; }
  int GetNumberOfVehicles() const { return number_of_vehicles_; }
  double GetMaxTime() const { return max_time_; }
  std::vector<Client> GetClients() const { return clients_; }
 private:
  int number_of_nodes_;
  int number_of_vehicles_;
  double max_time_;
  std::vector<Client> clients_;
};

inline std::ostream& operator<<(std::ostream& os, const DTOPInstance& instance) {
  os << "-------------------------------------------------------\n"
     << "Número de nodos: " << instance.GetNumberOfNodes() << std::endl
     << "Número de vehículos: " << instance.GetNumberOfVehicles() << std::endl
     << "Tiempo máximo: " << instance.GetMaxTime() << std::endl
     << "Clientes:\n";
  for (const Client& client : instance.GetClients()) {
    os << "  - Cliente " << client.id << ": \n"
       << "    Coordenadas: (" << client.x_coordinate << "," << client.y_coordinate  << ")"
       << "    Recompensa: " << client.reward
       << "    Tiempo de aparición: " << client.arrival_time << std::endl;
  }
  return os;
}

#endif