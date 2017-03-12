#include "laplace/config/mpi_config.h"
#include "laplace/math/vec_types.h"
#include "laplace/mpi/torus_3d_comm.h"
#include "laplace/utils/openmp.h"

#include <fmt/format.h>
#include <yaml-cpp/yaml.h>
#include <docoptcpp/docopt.h>
#include <mpi.h>
#include <iostream>

using namespace std;
using namespace laplace;

static const char USAGE_STRING[] =
R"(
la+

  Usage:
    la+ mdrun --config <config_file> [options]
    la+ (-h | --help)
    la+ --version

  Options:
    -h --help               Show this screen.
    -v --version            Show version.
    --config <config_file>  MD run configuration (YAML).
    --verbose               Print more logs.
)";

int main(int argc, char **argv) {
  auto args = docopt::docopt(USAGE_STRING,
                             { argv + 1, argv + argc },
                             true,                  // show help if requested
                             fmt::format("la+ {} ({})",
                                         LIB_VERSION,
                                         LIB_GIT_SHA));

  MPI_Init(&argc, &argv);

  if (mpi::mpi_rank(MPI_COMM_WORLD) == 0) {
    YAML::Node config = YAML::LoadFile( args["--config"].asString() );
    cout << config["config"]["integrator"] << endl;

    auto mpiconf = config["mpi"].as<config::MPIConfig>();
    fmt::print("dims are {} {} {}\n\n", mpiconf.dimensions[0], mpiconf.dimensions[1], mpiconf.dimensions[2]);
    cout << "loaded the config:\n" << config << endl;

    config::MPIConfig foo;
    YAML::Node out;
    out["mpi"] = foo;
    cout << out << endl;

    auto convertback = out["mpi"].as<config::MPIConfig>();
    fmt::print("dims are {} {} {}\n\n", convertback.dimensions[0], convertback.dimensions[1], convertback.dimensions[2]);

    fmt::print("nprocs is {}\n", openmp::get_num_procs());
  }

  auto func = []() {
    int dims[3] = { 2, 2, 1 };
    mpi::torus_3d_comm comm(dims, MPI_COMM_WORLD);
    fmt::print("Proc {} says Don't panic\n", mpi::mpi_rank(MPI_COMM_WORLD));
  };

  func();



  RVec x { 1, 2, 3.24 };
  fmt::print("rvec size returns {}\n", sizeof(rvec));
  fmt::print("RVec size returns {}\n", sizeof(RVec));
  fmt::print("x {}\n", x[2]);

  MPI_Finalize();
  return 0;
}
