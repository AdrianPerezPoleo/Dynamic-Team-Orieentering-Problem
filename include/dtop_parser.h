/**
 * @file dtop_parser.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Definition of a DTOP parser.
 * @version 1.0
 * @date 2026-10-24
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef DTOP_PARSER_H
#define DTOP_PARSER_H

#include "dtop_instance.h"

class DTOPParser {
 public:
  static DTOPInstance Parse(std::string& input_file);

 private:
  static int ParseNumberOfClients(std::istream& is);
  static int ParseNumberOfVehicles(std::istream& is);
  static double ParseMaxTime(std::istream& is);
  static std::vector<Client> ParseClients(std::istream& is);
};

#endif