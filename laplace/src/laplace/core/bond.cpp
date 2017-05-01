#include "laplace/core/bond.h"
#include <cmath>

using namespace std;

void laplace::bond_forces_update(MmSystem &system,
                                 const real L[3]) {
  auto &positions = system.atoms.positions;
  auto &bonds = system.bonds;
  auto &forces = system.atoms.forces;

  #pragma omp parallel for
  for (auto i = 0U; i < bonds.size(); ++i) {
    auto &bond = bonds[i];
    auto idx1 = bond.ids[0];
    auto idx2 = bond.ids[1];

    real dx = remainder(positions[idx1][0] - positions[idx2][0], L[0]);
    real dy = remainder(positions[idx1][1] - positions[idx2][1], L[1]);
    real dz = remainder(positions[idx1][2] - positions[idx2][2], L[2]);
    real r = sqrt(dx * dx + dy * dy + dz * dz);

    // harmonic bond force
    real f = 2. * bond.konst * (bond.r0 - r) / r;
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
