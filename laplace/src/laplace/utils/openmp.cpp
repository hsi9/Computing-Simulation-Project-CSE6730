#ifdef LAP_OPENMP
#include <omp.h>
#endif

#include "laplace/utils/openmp.h"

namespace openmp = laplace::openmp;

int openmp::get_max_threads(void) {
#if LAP_OPENMP
  return omp_get_max_threads();
#else
  return 1;
#endif
}

int openmp::get_num_procs(void) {
#if LAP_OPENMP
  return omp_get_num_procs();
#else
  return 1;
#endif
}

int openmp::get_thread_num(void) {
#if LAP_OPENMP
  return omp_get_thread_num();
#else
  return 0;
#endif
}

void openmp::set_num_threads(int num_threads) {
#if LAP_OPENMP
  omp_set_num_threads(num_threads);
#else
  return;
#endif
}
