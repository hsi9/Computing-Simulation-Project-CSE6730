#include "laplace/core/neighbor_search.h"

using namespace std;
using namespace laplace;

void laplace::perform_neighbor_search(vector<array<int, 2>> &pairs,
                                      const MmSystem &system,
                                      const real L[3],
                                      const int boxdims[3],
                                      const real cutoff2) {
  // CLEAR EXISTING PAIRS!
  pairs.clear();
  auto &positions = system.atoms.positions;

  // allocate implied linked list
  static vector<int> next;
  next.resize(positions.size());

  struct box b[boxdims[0]][boxdims[1]][boxdims[2]];

  // traverse all particles and assign to boxes
  for (auto i=0u; i < positions.size(); i++) {
    // initialize entry of implied linked list
    next[i] = -1;

    // which box does the particle belong to?
    // assumes particles have positions within [0,L]^3
    auto idx = (int)(positions[i][0]/L[0]*boxdims[0])%boxdims[0];
    auto idy = (int)(positions[i][1]/L[1]*boxdims[1])%boxdims[1];
    auto idz = (int)(positions[i][2]/L[2]*boxdims[2])%boxdims[2];

    // add to beginning of implied linked list
    auto bp = &b[idx][idy][idz];
    next[i] = bp->head;
    bp->head = i;
  }

  real d2;
  for (auto idx=0; idx<boxdims[0]; idx++) {
    for (auto idy=0; idy<boxdims[1]; idy++) {
      for (auto idz=0; idz<boxdims[2]; idz++) {
        auto bp = &b[idx][idy][idz];

        // within box interactions
        auto p1 = bp->head;
        while (p1 != -1) {
          auto p2 = next[p1];
          while (p2 != -1) {
            // do not need minimum image since we are in same box
            auto dx = positions[p1][0] - positions[p2][0];
            auto dy = positions[p1][1] - positions[p2][1];
            auto dz = positions[p1][2] - positions[p2][2];

            // if the distance is within the threshold, then add to pairs
            if ((d2 = dx*dx+dy*dy+dz*dz) < cutoff2) {
              pairs.emplace_back(array<int, 2>{ p1+1, p2+1 });
            }

            p2 = next[p2];
          }
          p1 = next[p1];
        }

        // interactions with other boxes
        for (auto j=0; j < NUM_BOX_NEIGHBORS; j++) {
          // get the neighbor box
          auto neigh_idx = (idx + box_neighbors[j][0] + boxdims[0]) % boxdims[0];
          auto neigh_idy = (idy + box_neighbors[j][1] + boxdims[1]) % boxdims[1];
          auto neigh_idz = (idz + box_neighbors[j][2] + boxdims[2]) % boxdims[2];
          auto neigh_bp = &b[neigh_idx][neigh_idy][neigh_idz];

          // when using boxes, the minimum image computation is
          // known beforehand, thus we can compute position offsets
          // to compensate for wraparound when computing distances
          real xoffset = 0.;
          real yoffset = 0.;
          real zoffset = 0.;
          if (idx + box_neighbors[j][0] == -1)         xoffset = -L[0];
          if (idy + box_neighbors[j][1] == -1)         yoffset = -L[1];
          if (idz + box_neighbors[j][2] == -1)         zoffset = -L[2];
          if (idx + box_neighbors[j][0] == boxdims[0]) xoffset =  L[0];
          if (idy + box_neighbors[j][1] == boxdims[1]) yoffset =  L[1];
          if (idz + box_neighbors[j][2] == boxdims[2]) zoffset =  L[2];

          p1 = neigh_bp->head;
          while (p1 != -1) {
            auto p2 = bp->head;
            while (p2 != -1) {
              // compute distance vector
              auto dx = positions[p1][0] - positions[p2][0] + xoffset;
              auto dy = positions[p1][1] - positions[p2][1] + yoffset;
              auto dz = positions[p1][2] - positions[p2][2] + zoffset;

              // if the distance is within the threshold, then add to pairs
              // check for p1 < p2 to avoid duplicate pairs
              if ((d2 = dx*dx+dy*dy+dz*dz) < cutoff2 and p1 < p2) {
                pairs.emplace_back(array<int, 2>{ p1+1, p2+1 });
              }

              p2 = next[p2];
            }
            p1 = next[p1];
          }
        }
      }
    }
  }
}
