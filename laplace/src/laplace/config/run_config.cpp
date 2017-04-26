#include "laplace/config/run_config.h"

using namespace laplace;
using RunConfig = laplace::config::RunConfig;

constexpr auto _INTEGRATOR = "integrator";
constexpr auto _STEPS = "steps";
constexpr auto _TINIT = "tinit";
constexpr auto _DT = "dt";

std::ostream& config::operator<<(std::ostream &os, const RunConfig &c) {
  YAML::Node node;
  node["RunConfig"] = c;
  YAML::Emitter out;
  out << node;
  return os << out.c_str();
}

bool RunConfig::operator==(RunConfig other) const {
  return integrator == other.integrator and
         steps == other.steps and
         dt == other.dt;
}

YAML::Node YAML::convert<RunConfig>::encode(const RunConfig &c) {
  Node node;
  node[_INTEGRATOR] = enumToString(c.integrator);
  node[_STEPS] = c.steps;
  node[_DT] = c.dt;
  return node;
}

bool YAML::convert<RunConfig>::decode(const Node &node, RunConfig &c) {
  if (not node.IsMap()) {
    return false;
  }

  auto integrator = node[_INTEGRATOR];
  if (integrator.IsScalar()) {
    auto s = integrator.as<std::string>();
    if (s == "VELOCITY_VERLET") c.integrator = config::IntegratorType::VELOCITY_VERLET;
  } else {
    return false;
  }

  auto steps = node[_STEPS];
  if (steps.IsScalar()) {
    c.steps = steps.as<int>();
  } else {
    return false;
  }

  auto tinit = node[_TINIT];
  if (tinit.IsScalar()) {
    c.tinit = tinit.as<real>();
  } else {
    return false;
  }

  auto dt = node[_DT];
  if (dt.IsScalar()) {
    c.dt = dt.as<real>();
  } else {
    return false;
  }

  return true;
}
