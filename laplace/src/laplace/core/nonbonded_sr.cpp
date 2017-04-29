#include "laplace/core/nonbonded_sr.h"
#include <cmath>

using namespace std;

void laplace::compute_distance2(vector<real> &distances2,
                                const MmSystem &system,
                                const vector<array<int, 2>> &pairs,
                                const real L[3]) {
  auto &positions = system.atoms.positions;
  distances2.clear();

  for (auto i=0u; i < pairs.size(); i++) {
    auto idx1 = pairs[i][0];
    auto idx2 = pairs[i][1];

    real dx = remainder(positions[idx1][0] - positions[idx2][0], L[0]);
    real dy = remainder(positions[idx1][1] - positions[idx2][1], L[1]);
    real dz = remainder(positions[idx1][2] - positions[idx2][2], L[2]);
    distances2.emplace_back(dx*dx + dy*dy + dz*dz);
  }
  return;
}

void laplace::nonbonded_sr_forces_update(MmSystem &system,
                                         const vector<array<int, 2>> &pairs,
                                         const real L[3],
                                         const real r_cut2) {
  auto &positions = system.atoms.positions;
  auto &forces = system.atoms.forces;
  auto &sigma = system.atoms.sigma;
  auto &epsilon = system.atoms.epsilon;

  #pragma omp parallel for
  for (auto i=0u; i < pairs.size(); i++) {
    auto idx1 = pairs[i][0];
    auto idx2 = pairs[i][1];

    /*
      It turns out that the remainder function is suited for minimum image
      convention, because the remainder is rounded to the nearest integral
      number, as opposed to fmod (which rounds the remainder towards zero)

      remainder(1.0, 10.0) => 1.0
      remainder(5.0, 10.0) => 5.0
      remainder(6.0, 10.0) => -4.0
      remainder(9.0, 10.0) => -1.0
    */
    real dx = remainder(positions[idx1][0] - positions[idx2][0], L[0]);
    real dy = remainder(positions[idx1][1] - positions[idx2][1], L[1]);
    real dz = remainder(positions[idx1][2] - positions[idx2][2], L[2]);
    real r2 = dx*dx + dy*dy + dz*dz;

    if (r2 <= r_cut2) {
      real _sigma = (real)0.5 * (sigma[idx1] + sigma[idx2]);
      real _epsilon = sqrt(epsilon[idx1] * epsilon[idx2]);
      real s = _sigma * _sigma / r2;
      s = s * s * s;

      // lennard-jones force
      real f = 24.0 * _epsilon * s / r2 * (1. - 2. * s);

      real fx = f * dx;
      real fy = f * dy;
      real fz = f * dz;

      #pragma omp atomic
      forces[idx1][0] += fx;
      #pragma omp atomic
      forces[idx2][0] -= fx;
      #pragma omp atomic
      forces[idx1][1] += fy;
      #pragma omp atomic
      forces[idx2][1] -= fy;
      #pragma omp atomic
      forces[idx1][2] += fz;
      #pragma omp atomic
      forces[idx2][2] -= fz;
    }
  }
}
