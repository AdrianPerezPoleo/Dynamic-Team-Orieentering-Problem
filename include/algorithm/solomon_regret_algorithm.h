/**
 * @file solomon_algorithm.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Definition of a Greedy algorithm based in the Solomon approach for the VRSPTW
 * @version 1.0
 * @date 27-10-2026
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef SOLOMON_ALGORITHM_H
#define SOLOMON_ALGORITHM_H

#include "../dtop_algorithm.h"

class SolomonRegretAlgorithm : public DTOPAlgorithm {
 public:
  DTOPSolution Solve(const DTOPInstance&) const override;

 private:
  int SelectInitialNode(const std::vector<double>&, const std::vector<bool>&) const;
  double CalculateInsertionCost(const Client&, const Client&, const Client&, double, double) const;
  std::vector<int> GetNearestNeighbors(const std::vector<double>&, const std::vector<bool>&, int) const;
};

#endif