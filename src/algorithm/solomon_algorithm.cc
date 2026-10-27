/**
 * @file solomon_algorithm.cc
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Implementation of the Solomon VRSPTW based greedy algorithm.
 * @version 1.0
 * @date 27-10-2026
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../../include/algorithm/solomon_algorithm.h"

/**
 * @brief 
 * 
 * @param instance 
 * @return DTOPSolution 
 */
DTOPSolution SolomonAlgorithm::Solve(const DTOPInstance& instance) const {
  // Para cada camión 
    // Elegir cliente inicial de la ruta (por ejemplo el cliente más lejano del almacén)
    // Para cada cliente que no tenga ruta asignada (En un futuro usar parametro q para no probar todos)
      // Obtener cuál sería su mejor lugar para insertar en la ruta actual
    // Insertar el cliente que tiene mejor desempeño -> Utilizar el arrepentimiento.
    // Si no hay más clientes con inserción factible -> Parar la ruta actual y continuar con la siguiente.
}