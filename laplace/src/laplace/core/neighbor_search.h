#ifndef LAP_CORE_NEIGHBOR_SEARCH_H
#define LAP_CORE_NEIGHBOR_SEARCH_H

#include "laplace/laplacepre.h"
#include "laplace/core/mm_system.h"

namespace laplace {
  constexpr int NUM_BOX_NEIGHBORS = 13;

  constexpr int box_neighbors[NUM_BOX_NEIGHBORS][3] = {
    {-1,-1,-1},
    {-1,-1, 0},
    {-1,-1,+1},
    {-1, 0,-1},
    {-1, 0, 0},
    {-1, 0,+1},
    {-1,+1,-1},
    {-1,+1, 0},
    {-1,+1,+1},
    { 0,-1,-1},
    { 0,-1, 0},
    { 0,-1,+1},
    { 0, 0,-1}
  };

  struct box {
    // default-initialize to -1
    int head = -1;
  };

  void perform_neighbor_search(std::vector<std::array<int, 2>> &pairs,
                               const MmSystem &system,
                               const real L[3],
                               const int boxdims[3],
                               const real cutoff2);
}

#endif
