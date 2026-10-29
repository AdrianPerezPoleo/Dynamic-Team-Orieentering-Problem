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
#include "../dtop_solution_builder.h"

struct InsertionData {
  int id = -1;
  double best_insertion_cost = 1e6;
  double second_best_insertion_cost = 1e6;
  int predecessor;
};

class SolomonRegretAlgorithm : public DTOPAlgorithm {
 public:
  DTOPSolution Solve(const DTOPInstance&) const override;

 private:
  int SelectInitialNode(const std::vector<double>&, const DTOPSolutionBuilder&) const;
  double CalculateInsertionCost(int, int, int, const std::vector<std::vector<double>>&, double, double) const;
  std::vector<int> GetNearestNeighbors(const std::vector<double>&, const DTOPSolutionBuilder&, int) const;
  void EvaluateInsertions(int route, const std::vector<int>& candidates, 
                          const DTOPSolutionBuilder& solution, const DTOPInstance& instance, 
                          std::vector<InsertionData>& insertion_data) const;
  int SelectBestCandidateByRegret(const std::vector<InsertionData>& insertion_data) const;
};

#endif