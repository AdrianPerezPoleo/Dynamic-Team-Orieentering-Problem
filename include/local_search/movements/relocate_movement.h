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
  RelocateMovement(int old_route, int new_route, int client_id, int new_predecessor,
        double removal_time_saved, double insertion_time_cost, double reward) : 
    old_route_{old_route}, new_route_{new_route}, client_id_{client_id}, new_predecessor_{new_predecessor},
    removal_time_saved_{removal_time_saved}, insertion_time_cost_{insertion_time_cost}, reward_{reward} { }

  void Execute(DTOPSolution& solution) const override {
    solution.RemoveClient(old_route_, client_id_, removal_time_saved_, reward_);
    solution.InsertClient(new_route_, client_id_, new_predecessor_, insertion_time_cost_, reward_);
  }
 private:
  int old_route_;  // Intra-route Relocate when old_route_ = new_route_
  int new_route_;
  int client_id_;
  int new_predecessor_;
  double removal_time_saved_;
  double insertion_time_cost_;
  double reward_;
};

#endif