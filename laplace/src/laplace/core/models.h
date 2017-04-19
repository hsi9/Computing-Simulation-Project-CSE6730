#ifndef LAP_CORE_MODELS_H
#define LAP_CORE_MODELS_H

#include "laplace/laplacepre.h"
#include "laplace/math/vec_types.h"

namespace laplace {
  struct Atoms {
    std::vector<RVec> positions;
    std::vector<RVec> velocities;
    std::vector<RVec> forces;
    std::vector<RVec> forces_old;
    std::vector<real> masses;
    std::vector<real> sigma;
    std::vector<real> epsilon;

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

  struct Bond {
    real epsilon;
    real r0;
    int ids[2];
  };

  struct Angle {
    real epsilon;
    real theta0;
    int ids[3];
  };

  struct Torsion {
    real energy;
    real phase;
    int ids[4];
    int overlaps;
    int symmetry;
    int nb14;
  };

  struct NonBond14 {
    int ids[2];
    int atom_types[2];
    real coeff_vdw;
    real coeff_ele;
  };
}

#endif
