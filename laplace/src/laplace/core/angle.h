#ifndef LAP_CORE_ANGLE_H
#define LAP_CORE_ANGLE_H

#include "laplace/laplacepre.h"
#include "laplace/core/mm_system.h"

using namespace std;

namespace laplace {

  void angle_forces_update(MmSystem &system,
                           const real L[3]);

}

#endif
