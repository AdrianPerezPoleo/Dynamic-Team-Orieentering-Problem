/**
 * @file swap_movement.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief 
 * @version 1.0
 * @date 2026-11-02
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef SWAP_MOVEMENT_H
#define SWAP_MOVEMENT_H

#include "../../dtop_solution.h"
#include "../route_movement.h"

class SwapMovement : public RouteMovement {
 public:
  struct NodeData {
    int route;
    int client_id;
    int predecessor_after_swap;
    double removal_time_saved;
    double insertion_time_cost;
    double reward;
  };

  SwapMovement(const NodeData& node1, const NodeData& node2) : node1_{node1}, node2_{node2} {}

  void Execute(DTOPSolution& solution) const override {
    solution.RemoveClient(node1_.route, node1_.client_id, node1_.removal_time_saved, node1_.reward);
    solution.RemoveClient(node2_.route, node2_.client_id, node2_.removal_time_saved, node2_.reward);

    solution.InsertClient(node2_.route, node1_.client_id, node1_.predecessor_after_swap, node1_.insertion_time_cost, node1_.reward);
    solution.InsertClient(node1_.route, node2_.client_id, node2_.predecessor_after_swap, node2_.insertion_time_cost, node2_.reward);
  }
 private:
  NodeData node1_;
  NodeData node2_;
};

#endif