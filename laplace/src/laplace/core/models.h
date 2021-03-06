#ifndef LAP_CORE_MODELS_H
#define LAP_CORE_MODELS_H

#include "laplace/laplacepre.h"
#include "laplace/math/vec_types.h"
#include <H5Cpp.h>

namespace laplace {
  struct Atoms {
    std::vector<RVec> positions;
    std::vector<RVec> velocities;
    std::vector<RVec> forces;
    std::vector<RVec> forces_old;
    std::vector<real> masses;
    std::vector<real> sigma;
    std::vector<real> epsilon;
    std::vector<real> q;

    inline void resize(const size_t n) {
      positions.resize(n);
      velocities.resize(n);
      forces.resize(n);
      forces_old.resize(n);
      masses.resize(n);
      sigma.resize(n);
      epsilon.resize(n);
      q.resize(n);
    }

    void load_topology(const H5::H5File &h5File);
    void load_latest_trajectory_snapshot(const H5::H5File &h5File);
    void load_trajectory(const H5::H5File &h5File,
                         const std::string &dataset);
    void print_info() const;
    void write_trajectory_snapshot(H5::H5File &h5File,
                                   const int64_t timestep) const;
  };

  /*
    http://www.ks.uiuc.edu/Training/Workshop/SanFrancisco/lectures/Wednesday-ForceFields.pdf
    http://cbio.bmt.tue.nl/pumma/index.php/Theory/Potentials
    https://udel.edu/~arthij/MD.pdf
  */

  struct Bond {
    real konst = 0.;
    real r0 = 0.;
    int ids[2];

    static H5::CompType h5_type();
    static std::string header_str();

    std::string info_str() const;
  };

  struct Angle {
    real konst = 0.;
    real theta0 = 0.;
    int ids[3];

    static H5::CompType h5_type();
    static std::string header_str();

    std::string info_str() const;
  };

  struct Torsion {
    real konst = 0.;
    real phi0 = 0.;
    int ids[4];
    int multiplicity = 0;

    static H5::CompType h5_type();
    static std::string header_str();

    std::string info_str() const;
  };

  struct NonBonded14 {
    int ids[2];
    real coeff_vdw;
    real coeff_ele;

    static H5::CompType h5_type();
    static std::string header_str();

    std::string info_str() const;
  };
}

#endif
