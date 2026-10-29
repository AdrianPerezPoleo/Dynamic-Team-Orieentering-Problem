/**
 * @file dtop_instance.cc
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Implementatin of the private methods of DTOPInstance.
 * @version 1.0
 * @date 28-10-2026
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <cmath>
#include "../include/dtop_instance.h"

void DTOPInstance::InitializeDistances() {
  for (const Client& client_1 : clients_) {
    for (const Client& client_2 : clients_) {
      distances_[client_1.id][client_2.id] = std::hypot(
        client_1.x_coordinate - client_2.x_coordinate,
        client_1.y_coordinate - client_2.y_coordinate
      );
    }  
  }
}