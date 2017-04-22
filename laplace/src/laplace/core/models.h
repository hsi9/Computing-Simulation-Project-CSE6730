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
    }
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
  };

  struct Angle {
    real konst = 0.;
    real theta0 = 0.;
    int ids[3];

    static H5::CompType h5_type();
  };

  struct Torsion {
    real konst = 0.;
    real phi0 = 0.;
    int ids[4];
    int multiplicity = 0;

    static H5::CompType h5_type();
  };

  struct NonBonded14 {
    int ids[2];
    real coeff_vdw;
    real coeff_ele;

    static H5::CompType h5_type();
  };
}

#endif
