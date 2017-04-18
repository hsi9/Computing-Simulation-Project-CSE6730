#include "laplace/config/t_coupling_config.h"
#include "laplace/utils/string.h"
#include <sstream>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gtest/gtest.h>

using namespace std;

TEST(TCouplingConfigTest, load_config) {
  YAML::Node yaml_config = YAML::Load(
    laplace::util::string::strip_margin(
      R"(
        |tcoupling:
        |  thermostat: SCALING
        |  coupling_freq: 12
        |  nh_chain_length: 42
        |)"
    )
  );
  auto conf = yaml_config["tcoupling"].as<laplace::config::TCouplingConfig>();
  fmt::print("{}\n", conf);
}

TEST(TCouplingTest, conversion_is_bidirectional) {
  laplace::config::TCouplingConfig conf;
  conf.thermostat = laplace::config::ThermostatType::HOOVER_EVANS;
  conf.nh_chain_length = 11;
  conf.coupling_freq = 27;

  YAML::Node out_yml;
  out_yml["tcoupling"] = conf;

  stringstream ss;
  ss << out_yml;

  YAML::Node in_yml = YAML::Load(ss.str());
  auto conf2 = in_yml["tcoupling"].as<laplace::config::TCouplingConfig>();
  EXPECT_EQ(conf, conf2);
}
