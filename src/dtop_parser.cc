/**
 * @file dtop_parser.cc
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Implementation of the DTOP parser.
 * @version 1.0
 * @date 2026-10-24
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../include/dtop_parser.h"
#include <fstream>
#include <string>
#include <algorithm>

/**
 * @brief Parses a file with the information of a DTOP instance.
 * 
 * @param is INput stram of the file.
 * @return DTOPInstance contained in the file.
 */
DTOPInstance DTOPParser::Parse(std::string& input_file) {
  std::ifstream is(input_file);
  int num_clients = ParseNumberOfClients(is);
  int num_vehicles = ParseNumberOfVehicles(is);
  double max_time = ParseMaxTime(is);
  auto clients = ParseClients(is);
  // std::sort(clients.begin(), clients.end());
  
  return DTOPInstance(num_clients, num_vehicles, max_time, clients);
}

/**
 * @brief Reads the number of clients the file contains.
 * 
 * @param is Input stream of the instance file.
 * @return int Number of clients the instance has.
 */
int DTOPParser::ParseNumberOfClients(std::istream& is) {
  std::string label;
  int n;
  is >> label >> n; 
  return n;
}

/**
 * @brief Reads the number of vehicles the file contains.
 * 
 * @param is Input stream of the instance file.
 * @return int Number of vehicles the instance has.
 */
int DTOPParser::ParseNumberOfVehicles(std::istream& is) {
  std::string label;
  int m;
  is >> label >> m; 
  return m;
}

/**
 * @brief Reads the max time the problem will have.
 * 
 * @param is Input stream of the instance file.
 * @return double Max time the problem instance has.
 */
double DTOPParser::ParseMaxTime(std::istream& is) {
  std::string label;
  double tmax;
  is >> label >> tmax; 
  return tmax;
}

/**
 * @brief Reads the information about the clients the file contains.
 * 
 * @param is Input stream of the instance file.
 * @return std::vector<Client> Vector containing the clients the instance has.
 */
std::vector<Client> DTOPParser::ParseClients(std::istream& is) {
  std::vector<Client> clients;
  double x, y, disclosure_time;
  int profit;
  int current_id = 0;

  while (is >> x >> y >> profit >> disclosure_time) {
    clients.emplace_back(current_id, x, y, profit, disclosure_time);
    ++current_id;
  }
    
  return clients;
}