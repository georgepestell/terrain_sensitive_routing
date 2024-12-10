#pragma once

#include "tsr/Delaunay_3.hpp"
#include "tsr/Feature.hpp"
#include "tsr/Point_3.hpp"
#include <cmath>
#include <memory>
#include <string>
#include <sys/types.h>
#include <vector>

namespace tsr {

class GradientSpeedFeature : public Feature<double> {
private:
  std::vector<double> upwards_coefficients;
  std::vector<double> downwards_coefficients;

  enum DEPENDENCIES { X };

  static inline std::vector<double> DEFAULT_UPWARDS_COEFFS = {1, -2.7, -34.83,
                                                              200.63, -292.06};
  static inline std::vector<double> DEFAULT_DOWNWARDS_COEFFS = {
      1, -0.01, 79.31, 1164.83, 4622.34, 5737.68};

public:
  GradientSpeedFeature(std::string name,
                       std::vector<double> upwards_coefficients,
                       std::vector<double> downwards_coefficients)
      : Feature<double>(name), upwards_coefficients(upwards_coefficients),
        downwards_coefficients(downwards_coefficients) {};

  GradientSpeedFeature(std::string name)
      : GradientSpeedFeature(name, DEFAULT_UPWARDS_COEFFS,
                             DEFAULT_DOWNWARDS_COEFFS) {}

  static double solvePolynomial(double x, std::vector<double> &coefficients) {

    double y = 0;
    for (uint degree = 0; degree < coefficients.size(); degree++) {
      y += std::pow(x, degree);
    }

    return y;
  }

  double calculate(Face_handle face, Point_3 &source_point,
                   Point_3 &target_point) override {

    // Solve the polynomial with the given input
    auto inputFeature = dynamic_pointer_cast<Feature<double>>(
        this->dependencies[DEPENDENCIES::X]);

    // Get the dependency value
    double gradient = inputFeature->calculate(face, source_point, target_point);

    double speedInfluence;
    if (gradient > 0) {
      speedInfluence = solvePolynomial(gradient, this->upwards_coefficients);
    } else {
      speedInfluence = solvePolynomial(gradient, this->downwards_coefficients);
    }

    return speedInfluence < 0 ? 0 : speedInfluence;
  }
};

}; // namespace tsr