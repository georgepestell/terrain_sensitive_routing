#pragma once

#include "tsr/Delaunay_3.hpp"
#include "tsr/Feature.hpp"
#include "tsr/Point_3.hpp"
#include "tsr/logging.hpp"
#include <boost/concept_check.hpp>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace tsr {

class MultiplierFeature : public Feature<double> {
public:
  enum DEPENDENCY_TYPE { INT, DOUBLE, BOOL };

  std::unordered_map<std::string, DEPENDENCY_TYPE> dependency_types;

  using Feature<double>::Feature;

  double calculate(TSRState &state) override {

    double total = 1;
    for (auto f : this->dependencies) {
      switch (this->dependency_types[f->featureID]) {
      case INT: {
        auto feature = std::dynamic_pointer_cast<Feature<int>>(f);
        int value = feature->calculate(state);

        if (total == std::numeric_limits<double>::infinity() ||
            ((double)value) == std::numeric_limits<double>::infinity()) {
          total = std::numeric_limits<double>::infinity();
          break;
        }

        total *= (double)value;
        break;
      }
      case DOUBLE: {
        auto feature = std::dynamic_pointer_cast<Feature<double>>(f);
        double value = feature->calculate(state);

        if (total == std::numeric_limits<double>::infinity() ||
            value == std::numeric_limits<double>::infinity()) {
          total = std::numeric_limits<double>::infinity();
          break;
        }

        total *= value;
        break;
      }
      case BOOL: {
        auto feature = std::dynamic_pointer_cast<Feature<bool>>(f);
        bool value = feature->calculate(state);
        if (value) {
          break;
        } else {
          return 0;
        }
        break;
      }
      default:
        TSR_LOG_ERROR("Multiplier feature type invalid");
        throw std::runtime_error("multiplier feature type invalid");
      }

      if (total == 0) {
        break;
      }
    }

    return total;
  }

  void add_dependency(std::shared_ptr<FeatureBase> feature) override {
    boost::ignore_unused_variable_warning(feature);
    TSR_LOG_ERROR(
        "Multiplier add_dependency requires specifying the feature type");
    throw std::runtime_error(
        "Multiplier add_dependency requires specifying the feature type");
  }

  void add_dependency(std::shared_ptr<FeatureBase> feature,
                      DEPENDENCY_TYPE type) {
    this->dependencies.push_back(feature);
    this->dependency_types[feature->featureID] = type;
  }
};

} // namespace tsr