#ifndef LAP_CORE_INTEGRATION_H
#define LAP_CORE_INTEGRATION_H

#include "laplace/laplacepre.h"

namespace laplace {
  void velocity_verlet_kernel(real *x_vec,
                              real *v_vec,
                              real *f_vec,
                              real *f_vec_old,
                              real *half_dt_mass_inv,
                              const int size,
                              const real *box_dims,
                              const real dt);
}

#endif
