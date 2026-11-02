/**
 * @file relocate_movement.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Implementation of the Relocate movement. 
 * @version 1.0
 * @date 2026-10-31
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef RELOCATE_MOVEMENT_H
#define RELOCATE_MOVEMENT_H

#include "../../dtop_solution.h"
#include "../route_movement.h"

class RelocateMovement : public RouteMovement {
 public:
  struct RelocateData {
    int old_route;
    int new_route;
    int client_id;
    int new_predecessor;
    double removal_time_saved;
    double insertion_time_cost;
    double reward;
  };

  RelocateMovement(const RelocateData& data) : data_{data} { }

  void Execute(DTOPSolution& solution) const override {
    solution.RemoveClient(data_.old_route, data_.client_id, data_.removal_time_saved, data_.reward);
    solution.InsertClient(data_.new_route, data_.client_id, data_.new_predecessor, data_.insertion_time_cost, data_.reward);
  }

 private:
  RelocateData data_;
};

#endif