/**
 * @file route_movement.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Definition of an abstract class that represent a movement into a DTOP solution.
 * @version 1.0
 * @date 2026-10-31
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef ROUTE_MOVEMENT_H
#define ROUTE_MOVEMENT_H

#include "../dtop_solution.h"

class DTOPSolution;

class RouteMovement {
 public:
  virtual ~RouteMovement() = default;

  virtual void Execute(DTOPSolution& solution) const = 0;
};

#endif