#include "laplace/config/mpi_config.h"
#include "laplace/config/simulation_config.h"
#include "laplace/math/vec_types.h"
#include "laplace/mpi/torus_3d_comm.h"
#include "laplace/utils/openmp.h"
#include "laplace/hdf5/h5_type.h"
#include "laplace/hdf5/h5_read.h"
#include "laplace/hdf5/h5_util.h"
#include "laplace/core/mm_system.h"

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
    la+ --config <config_file> [options]
    la+ (-h | --help)
    la+ --version

  Options:
    -h --help               Show this screen.
    -v --version            Show version.
    --config <config_file>  MD run configuration (YAML).
    --data <data_file>      MD run system input data (HDF5).
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
    auto mmsystem = laplace::MmSystem::load_from_file(h5File);
    mmsystem.print_info();

/*
    auto r0s = laplace::hdf5::load_struct_array_column<double>(h5File, "topology/bonds", "r0", H5::PredType::NATIVE_DOUBLE);
    auto ds = laplace::hdf5::load_2d_array_n_columns<double, 4>(h5File, "trajectory/0000/rvf", H5::PredType::NATIVE_DOUBLE, 2);

    for (const auto &i : r0s) {
      cout << i << "\n";
    } cout << "\n";

    for (const auto &d : ds) {
      cout << d[0] << " " << d[1] << " " << d[2] << " " << d[3] << "\n";
    } cout << "\n";

    auto subgroups = laplace::hdf5::list_subgroups(h5File, "trajectory");
    for (const auto &s : subgroups) {
      cout << s << "\n";
    } cout << "\n";
*/

  } catch (const std::exception& e) {
    cout << e.what() << endl;
  }

/*
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
*/

  return 0;
}
