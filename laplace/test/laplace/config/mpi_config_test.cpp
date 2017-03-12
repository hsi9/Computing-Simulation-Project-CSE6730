#include "laplace/config/mpi_config.h"
#include "laplace/utils/string.h"
#include <sstream>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gtest/gtest.h>

using namespace std;

TEST(MPIConfigTest, load_config) {
  YAML::Node yaml_config = YAML::Load(
    laplace::util::string::strip_margin(
      R"(
        |mpi:
        |  dimensions: [1, 2, 3]
        |)"
    )
  );
  auto mpiconf = yaml_config["mpi"].as<laplace::config::MPIConfig>();
  fmt::print("{}\n", mpiconf);
}

TEST(MPIConfigTest, conversion_is_bidirectional) {
  laplace::config::MPIConfig config;
  config.dimensions[0] = 42;
  config.dimensions[0] = 2;
  config.dimensions[0] = 27;

  YAML::Node out_yml;
  out_yml["mpi"] = config;

  stringstream ss;
  ss << out_yml;

  YAML::Node in_yml = YAML::Load(ss.str());
  auto config2 = in_yml["mpi"].as<laplace::config::MPIConfig>();
  EXPECT_EQ(config, config2);
}
