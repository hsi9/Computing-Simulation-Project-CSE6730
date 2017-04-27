#ifndef LAP_CONFIG_RUN_CONFIG_H
#define LAP_CONFIG_RUN_CONFIG_H

#include "laplace/laplacepre.h"
#include <yaml-cpp/yaml.h>
#include <ostream>

namespace laplace {
namespace config {
  enum class IntegratorType {
    NONE,
    VELOCITY_VERLET
  };

  inline const char* enumToString(IntegratorType t) {
    switch (t) {
      case IntegratorType::VELOCITY_VERLET: return "VELOCITY_VERLET";
      default: return "NONE";
    }
  }

  struct RunConfig {
    IntegratorType integrator = IntegratorType::VELOCITY_VERLET;
    int steps = -1;
    real tinit = 0;
    real dt = 0.001;

    bool operator==(RunConfig other) const;
  };

  std::ostream& operator<<(std::ostream &, const RunConfig &);
}}

namespace YAML {
  using RunConfig = laplace::config::RunConfig;
  template<> struct convert<RunConfig> {
    static Node encode(const RunConfig &rhs);
    static bool decode(const Node &node, RunConfig &rhs);
  };
}

#endif
