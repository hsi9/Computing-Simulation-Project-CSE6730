#ifndef LAP_CORE_ATOMS_H
#define LAP_CORE_ATOMS_H

#include "laplace/laplacepre.h"
#include "laplace/core/models.h"

namespace laplace {
  struct MmSystem {
    Atoms atoms;
    std::vector<Bond> bonds;
    std::vector<Angle> angles;
    std::vector<Torsion> dihedrals;
    std::vector<Torsion> impropers;
    std::vector<NonBonded14> nonbonded14s;

    static MmSystem load_from_file(const std::string &filepath);
  };
}

#endif
