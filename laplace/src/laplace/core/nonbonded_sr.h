#ifndef LAP_CORE_NONBONDED_SR_H
#define LAP_CORE_NONBONDED_SR_H

#include "laplace/laplacepre.h"
#include "laplace/core/mm_system.h"

using namespace std;

namespace laplace {
  void nonbonded_sr_forces_update(MmSystem &system,
                                  const std::vector<std::array<int, 2>> &pairs,
                                  const std::vector<real> &distances2,
                                  const real L[3],
                                  const real r_cut2);
}

#endif
