#ifndef LAP_MPI_TORUS_3D_COMM_H
#define LAP_MPI_TORUS_3D_COMM_H

#include "laplace/laplacepre.h"
#include "laplace/mpi/mpi_utilities.h"
#include <mpi.h>
#include <memory>
#include <ostream>

namespace laplace {
namespace mpi {
  class torus_3d_comm {
    int dims[DIMS] = { 0, 0, 0 };

    // MPI_Comm is a pointer type; here we are computing the actual underlying type to parameterize unique_ptr
    std::unique_ptr<std::remove_reference<decltype(*MPI_Comm())>::type,
                    decltype(&free_mpi_comm)
                   > cartesian_comm_ { nullptr, free_mpi_comm };
  public:
    torus_3d_comm() = delete;
    torus_3d_comm(int _dims[DIMS], MPI_Comm world_comm = MPI_COMM_WORLD);

    friend std::ostream& operator<<(std::ostream &os, const torus_3d_comm &t);

    inline MPI_Comm raw() const {
      return cartesian_comm_.get();
    }
  };

  std::ostream& operator<<(std::ostream &, const torus_3d_comm &);
}}

#endif
