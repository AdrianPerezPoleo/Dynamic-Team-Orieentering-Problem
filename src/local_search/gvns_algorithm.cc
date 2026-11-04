/**
 * @file gvns_algorithm.cc
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Implementation of a GVNS algorithm to solve the DTOP.
 * @version 0.1
 * @date 2026-11-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../../include/local_search/gvns_algorithm.h"

/**
 * @brief 
 * 
 * @param instance 
 * @return DTOPSolution 
 */
DTOPSolution GVNSAlgorithm::Solve(const DTOPInstance& instance) const {
  int k = 100;
  DTOPSolution best_solution;

  for (int i{0}; i < k; ++i) {
    DTOPSolution current_solution = this->constructive_algorithm_.Solve(instance);
    if (current_solution.GetTotalProfit() > best_solution.GetTotalProfit()) {
        best_solution = current_solution;
    }
  }

  return best_solution;
}