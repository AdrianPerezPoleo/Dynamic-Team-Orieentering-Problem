/**
 * @file random_generator.h
 * @author Adrián Pérez Poleo (alu0101635224@ull.edu.es)
 * @brief Implementation of the Randomg Generator.
 * @version 1.0
 * @date 2026-10-30
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <random>
#include <vector>

class RandomGenerator {
 public:
  RandomGenerator(unsigned int seed = std::random_device{}()) : generator_(seed) {}

  int GetInt(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator_);
  }

  double GetDouble(double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator_);
  }

 private:
  std::mt19937 generator_;
};

#endif