/**
 * @file gvns_algorithm.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Definition of a GVNS algorithm to solve the DTOP.
 * @version 1.0
 * @date 2026-11-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef GVNS_ALGORITHM_H
#define GVNS_ALGORITHM_H

#include "../dtop_algorithm.h"

class GVNSAlgorithm : public DTOPAlgorithm {
 public:
  GVNSAlgorithm(const DTOPAlgorithm& constructive_algorithm) : constructive_algorithm_{constructive_algorithm} { }
  DTOPSolution Solve(const DTOPInstance&) const override;  
 private:
  const DTOPAlgorithm& constructive_algorithm_;
  // NeighborhoodSearchAlgorithm
};

#endif