#include "laplace/mpi/torus_3d_comm.h"

#include <fmt/format.h>
#include <exception>

namespace mpi = laplace::mpi;

std::ostream& mpi::operator<<(std::ostream &os, const mpi::torus_3d_comm &t) {
  return os << "torus_3d_comm(["
            << t.dims[0] << ','
            << t.dims[1] << ','
            << t.dims[2] << "])";
}

mpi::torus_3d_comm::torus_3d_comm(int _dims[DIMS], MPI_Comm world_comm) {
  int nprocs = num_procs(world_comm);
  if (nprocs != (_dims[0] * _dims[1] * _dims[2])) {
    throw std::invalid_argument(fmt::format(
      "incorect number of processors to span a {}x{}x{} torus topology (got {})",
      _dims[0], _dims[1], _dims[2], nprocs
    ));
  }

  for (auto i=0; i < DIMS; i++) dims[i] = _dims[i];

  int periods[DIMS] = { 1, 1, 1 };
  MPI_Comm raw_comm = nullptr;
  MPI_Cart_create(world_comm, DIMS, dims, periods, 1, &raw_comm);
  cartesian_comm_.reset(std::move(raw_comm));
}
