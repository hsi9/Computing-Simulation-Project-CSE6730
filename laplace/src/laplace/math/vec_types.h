#ifndef LAP_MATH_VECTYPES_H
#define LAP_MATH_VECTYPES_H

#include "laplace/laplacepre.h"

namespace laplace {
  using RVec = std::array<real, DIMS>;
  using rvec = real[DIMS];

  template<typename T, std::size_t N>
  T* as_c_array(std::vector<std::array<T, N>> &vec) {
    return static_cast<T*>(static_cast<void*>(&vec[0]));
  }
}

#endif
