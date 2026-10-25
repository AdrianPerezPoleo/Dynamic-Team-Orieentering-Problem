/**
 * @file dtop_algorithm.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Definition of an algorithm class for the DTOP problem.
 * @version 1.0
 * @date 2026-10-25
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef DTOP_ALGORITHM_H
#define DTOP_ALGORITHM_H

#include "dtop_instance.h"
#include "dtop_solution.h"

class DTOPAlgorithm {
 public:
  virtual ~DTOPAlgorithm() = default;

  virtual DTOPSolution Solve(const DTOPInstance&) const = 0;
  virtual DTOPSolution Solve(const DTOPInstance& instance, const DTOPSolution& initial_solution) const {
    return Solve(instance); 
  }
};

#endif