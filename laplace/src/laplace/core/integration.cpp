#include "laplace/core/integration.h"
#include <cmath>

using namespace laplace;

void laplace::velocity_verlet_kernel(real *x_vec,
                                     real *v_vec,
                                     real *f_vec,
                                     real *f_vec_old,
                                     real *half_dt_mass_inv,
                                     const int size,
                                     const real *L,
                                     const real dt) {
  // #pragma omp simd aligned(x_vec:32), aligned(v_vec:32), aligned(f_vec:32), aligned(f_vec_old:32), aligned(half_dt_mass_inv:32)
  #pragma omp parallel for
  for (auto i=0; i < size; i++) {
    v_vec[i] = half_dt_mass_inv[i] * (f_vec[i] + f_vec_old[i]);

    auto box_idx = i % DIMS;
    /*
      add L[box_idx] here so that negative positions can be positive
      i.e. in a box of L=10, and x = -1.0, then adding L will give x = 9.0
      similarly, for x = 5.0, adding L will give x = 15.0 (outside the box)
    */
    x_vec[i] += dt * (v_vec[i] + half_dt_mass_inv[i] * f_vec[i]);
    /*
      positions outside L will be re-normalized by remainder division (fmod)
      For example, for x = 15.0, fmod against 10 will give back x = 5.0
    */
    x_vec[i] = fmod(x_vec[i], L[box_idx]);
    if (x_vec[i] < 0) x_vec[i] +=  L[box_idx];

    f_vec_old[i] = f_vec[i];
  }
}
