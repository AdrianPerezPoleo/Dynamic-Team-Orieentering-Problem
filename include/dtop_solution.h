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

class DTOPSolution {
 public:

 private:
  std::vector<int> next_node_;
  std::vector<int> prev_node_;

  double total_profit_;
  std::vector<double> route_profit;

  std::vector<double> route_duration_;

  std::vector<bool> visited_;

  std::vector<std::vector<double>> arrival_times;
};

#endif