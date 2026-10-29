/**
 * @file main.cc
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Main function of the program.
 * @version 1.0
 * @date 2026-10-24
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../include/dtop_parser.h"
#include "../include/dtop_algorithm.h"
#include "../include/algorithm/solomon_regret_algorithm.h"
#include "../include/dtop_solution_renderer.h"

#include <chrono>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_instancia>" << std::endl;
    return 1;
  }

  std::string file_name = argv[1];
  DTOPInstance instance = DTOPParser::Parse(file_name);
  std::cout << instance << std::endl;

  DTOPAlgorithm* algorithm = new SolomonRegretAlgorithm();

  auto start = std::chrono::high_resolution_clock::now();
  DTOPSolution solution = algorithm->Solve(instance);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> duration = end - start;
  std::cout << "Tiempo de ejecucion: " << duration.count() << " ms" << std::endl;

  SolutionRenderer::ExportToGraphviz(solution, instance);
  SolutionRenderer::PrintConsoleSummary(solution, instance);
  return 0;
}