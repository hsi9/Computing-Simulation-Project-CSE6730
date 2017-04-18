#ifndef LAP_MPI_MPI_UTILS_H
#define LAP_MPI_MPI_UTILS_H

#include <mpi.h>

namespace laplace {
namespace mpi {
  inline decltype(auto) free_mpi_comm(MPI_Comm comm) {
    MPI_Comm_free(&comm);
  }

  inline decltype(auto) mpi_rank(MPI_Comm comm) {
    int rank; MPI_Comm_rank(comm, &rank);
    return rank;
  }

  inline decltype(auto) num_procs(MPI_Comm comm) {
    int nprocs; MPI_Comm_size(comm, &nprocs);
    return nprocs;
  }
}}

#endif
