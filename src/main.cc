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

int main(int argc, char* argv[]) {
  std::string file_name = argv[1];
  DTOPInstance instance = DTOPParser::Parse(file_name);

  std::cout << instance << std::endl;
}