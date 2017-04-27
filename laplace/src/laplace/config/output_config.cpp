#include "laplace/config/output_config.h"

using namespace laplace;
using OutputConfig = laplace::config::OutputConfig;

constexpr auto _INTERVAL = "interval";

std::ostream& config::operator<<(std::ostream &os, const OutputConfig &c) {
  YAML::Node node;
  node["OutputConfig"] = c;
  YAML::Emitter out;
  out << node;
  return os << out.c_str();
}

bool OutputConfig::operator==(OutputConfig other) const {
  return interval == other.interval;
}

YAML::Node YAML::convert<OutputConfig>::encode(const OutputConfig &c) {
  Node node;
  node[_INTERVAL] = c.interval;
  return node;
}

bool YAML::convert<OutputConfig>::decode(const Node &node, OutputConfig &c) {
  if (not node.IsMap()) {
    return false;
  }

  auto interval = node[_INTERVAL];
  if (interval.IsScalar()) {
    c.interval = interval.as<int>();
  } else {
    return false;
  }

  return true;
}
