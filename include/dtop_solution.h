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

#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>

#include "local_search/route_movement.h"

struct DTOPSolutionData {
    std::vector<int> next_node;
    std::vector<int> prev_node;
    std::vector<int> first_node;
    std::vector<int> last_node;
    std::vector<double> route_profit;
    std::vector<double> route_duration;
    std::vector<bool> visited;
    std::vector<std::vector<double>> arrival_times;
};

class DTOPSolution {
 public:
  DTOPSolution() = default;
  DTOPSolution(DTOPSolutionData data) : 
    next_node_{std::move(data.next_node)}, prev_node_{std::move(data.prev_node)},
    first_node_{std::move(data.first_node)}, last_node_{std::move(data.last_node)},
    route_profit_{std::move(data.route_profit)}, route_duration_{std::move(data.route_duration)}, 
    visited_{std::move(data.visited)}, arrival_times_{std::move(data.arrival_times)} { }

  double GetTotalProfit() const;
  void Execute(const RouteMovement& movement) { movement.Execute(*this); }
  
  const std::vector<int>& GetFirstNodes() const { return first_node_; }
  const std::vector<int>& GetNextNodes() const { return next_node_; }
  const std::vector<double>& GetRouteProfits() const { return route_profit_; }
  const std::vector<double>& GetRouteDurations() const { return route_duration_; }
  const std::vector<bool>& GetVisited() const { return visited_; }

  void InsertClient(int route, int candidate, int predecessor, double cost, double reward);
  void RemoveClient(int route, int client, double time_saved, double reward_lost);
  void ApplyTwoOpt(int route, int first_client, int last_client, double delta_time);
  double CalculateInsertionCost(int predecessor, int successor, int candidate, const std::vector<std::vector<double>>& distances) const;
  double CalculateRemovalCost(int predecessor, int client, int successor, const std::vector<std::vector<double>>& distances) const;

  DTOPSolution& operator=(const DTOPSolution&) = default;

 private:
  std::vector<int> next_node_;
  std::vector<int> prev_node_;

  std::vector<int> first_node_;  // First node of each route
  std::vector<int> last_node_;   // Last node of each route

  std::vector<double> route_profit_;
  std::vector<double> route_duration_;
  std::vector<bool> visited_;
  std::vector<std::vector<double>> arrival_times_;
};

#endif