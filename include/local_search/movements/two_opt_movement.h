/**
 * @file two_opt_movement.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief 
 * @version 1.0
 * @date 2026-11-02
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef TWO_OPT_MOVEMENT_H
#define TWO_OPT_MOVEMENT_H

#include "../../dtop_solution.h"
#include "../route_movement.h"

class TwoOptMovement : public RouteMovement {
 public:
  struct TwoOptData {
    int route;
    int first_client;
    int last_client;
    double delta_time;
  };

  TwoOptMovement(const TwoOptData& data) : data_{data} { }

  void Execute(DTOPSolution& solution) const override {
    solution.ApplyTwoOpt(data_.route, data_.first_client, data_.last_client, data_.delta_time);
  }

 private:
  TwoOptData data_;
};

#endif