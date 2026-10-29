/**
 * @file dtop_solution_renderer.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief 
 * @version 1.0
 * @date 29-10-2026
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef SOLUTION_RENDERER_H
#define SOLUTION_RENDERER_H

#include "dtop_solution.h"
#include "dtop_instance.h"

#include <string>
#include <fstream>
#include <vector>

class SolutionRenderer {
 public:
  static void PrintConsoleSummary(const DTOPSolution& solution, const DTOPInstance& instance);
  static void ExportToGraphviz(const DTOPSolution& solution, const DTOPInstance& instance, const std::string& filename = "rutas.dot");

 private:
  static void PrintSingleRoute(const DTOPSolution& solution, size_t route_index, int end_depot_id);

  static void WriteGraphvizHeader(std::ofstream& out);
  static void WriteGraphvizNodes(std::ofstream& out, const DTOPSolution& solution, const DTOPInstance& instance, int end_depot_id);
  static void WriteGraphvizRoutes(std::ofstream& out, const DTOPSolution& solution, const DTOPInstance& instance);
  static void CompileGraphviz(const std::string& filename);
  
  static constexpr double kNodeScale = 2.0;
  static inline const std::vector<std::string> kRouteColors = {
    "red", "blue", "darkorange", "purple", "brown", "darkgreen"
  };
};

#endif