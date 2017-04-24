#include "laplace/config/simulation_config.h"
#include "laplace/utils/openmp.h"
#include "laplace/core/mm_system.h"
#include <fmt/format.h>
#include <docoptcpp/docopt.h>
#include <iostream>

using namespace std;
using namespace laplace;

static const char USAGE_STRING[] =
R"(
la+

  Usage:
    la+ --config <config_file> [options]
    la+ (-h | --help)
    la+ --version

  Options:
    -h --help               Show this screen.
    -v --version            Show version.
    --config <config_file>  MD run configuration (YAML).
    --data <data_file>      MD run system input data (HDF5).
    --out <data_file>       Filepath for saving trajectories (HDF5).
    --verbose               Print more logs.
)";

int main(int argc, char **argv) {
  auto args = docopt::docopt(USAGE_STRING,
                             { argv + 1, argv + argc },
                             true,                  // show help if requested
                             fmt::format("la+ {} ({})",
                                         LIB_VERSION,
                                         LIB_GIT_SHA));
  try {
    auto config = config::SimulationConfig::load_from_file(args["--config"].asString());
    cout << config << endl;

    H5::H5File h5File(args["--data"].asString(), H5F_ACC_RDONLY);
    auto mmsystem = MmSystem::load_from_file(h5File);
    mmsystem.print_info();

    // copy the trajectory to two snapeshots in the outfile
    H5::H5File outFile(args["--out"].asString(), H5F_ACC_TRUNC);
    mmsystem.atoms.write_trajectory_snapshot(outFile, 42);
    mmsystem.atoms.write_trajectory_snapshot(outFile, 43);

  } catch (const std::exception& e) {
    cout << e.what() << endl;
  }

  return 0;
}
