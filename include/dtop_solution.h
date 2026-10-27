/**
 * @file dtop_solucion.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief 
 * @version 1.0
 * @date 2026-10-25
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef DTOP_SOLUTION_H
#define DTOP_SOLUTION_H

#include <vector>

struct DTOPSolutionData {
    std::vector<int> next_node;
    std::vector<int> prev_node;
    std::vector<double> route_profit;
    std::vector<double> route_duration;
    std::vector<bool> visited;
    std::vector<std::vector<double>> arrival_times;
};

class DTOPSolution {
 public:
  DTOPSolution(const DTOPSolutionData& data) : 
    next_node_{std::move(data.next_node)}, prev_node_{std::move(data.prev_node)}, 
    route_profit_{std::move(route_profit_)}, route_duration_{std::move(data.route_duration)}, 
    visited_{std::move(data.visited)}, arrival_times_{std::move(data.arrival_times)} { }

  double GetTotalProfit() const {
    double total_profit{0.0};
    for (const auto& route : route_profit_) {
      total_profit += route;
    }
    return total_profit;
  }

 private:
  std::vector<int> next_node_;
  std::vector<int> prev_node_;

  // double total_profit_;
  std::vector<double> route_profit_;

  std::vector<double> route_duration_;

  std::vector<bool> visited_;

  std::vector<std::vector<double>> arrival_times_;
};

#endif