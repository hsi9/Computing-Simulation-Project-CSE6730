#include "laplace/core/angle.h"
#include <cmath>

using namespace std;

// NOTE: Currently uses cosine harmonic potential.
// HARMONIC POTENTIAL IS CURRENTLY UNSUPPORTED.
void laplace::angle_forces_update(MmSystem &system,
                                  const real L[3]) {
  auto &positions = system.atoms.positions;
  auto &angles = system.angles;
  auto &forces = system.atoms.forces;

  #pragma omp parallel for
  for (auto i = 0U; i < angles.size(); ++i) {
    auto &angle = angles[i];

    auto idx1 = angle.ids[0];
    auto idx2 = angle.ids[1];
    auto idx3 = angle.ids[2];

    // r1 points from idx2 to idx1
    // r2 would be the zero vector, and is not a part of the calculation
    // r3 points from idx2 to idx3
    real dx1 = remainder(positions[idx2][0] - positions[idx1][0], L[0]);
    real dy1 = remainder(positions[idx2][1] - positions[idx1][1], L[1]);
    real dz1 = remainder(positions[idx2][2] - positions[idx1][2], L[2]);
    real dx3 = remainder(positions[idx2][0] - positions[idx3][0], L[0]);
    real dy3 = remainder(positions[idx2][1] - positions[idx3][1], L[1]);
    real dz3 = remainder(positions[idx2][2] - positions[idx3][2], L[2]);
    real r1sq = dx1 * dx1 + dy1 * dy1 + dz1 * dz1;
    real r3sq = dx3 * dx3 + dy3 * dy3 + dz3 * dz3;

    // inner product <r1, r3>
    real r1_dot_r3 = dx1 * dx3 + dy1 * dy3 + dz1 * dz3;

    // magnitude |r1| * |r3|
    real mag_r1_r3 = sqrt(r1sq * r3sq);

    // 2 k (cos(theta0) - cos(theta)) / (|r1| * |r3|)
    real coeff = 2. * angle.konst * (cos(angle.theta0) - (r1_dot_r3 / mag_r1_r3)) / mag_r1_r3;

    real inner_coeff1 = r1_dot_r3 / r1sq;
    real inner_coeff3 = r1_dot_r3 / r3sq;

    // cosine harmonic angle force
    real fx1 = coeff * (dx3 - inner_coeff1 * dx1);
    real fy1 = coeff * (dy3 - inner_coeff1 * dy1);
    real fz1 = coeff * (dz3 - inner_coeff1 * dz1);
    real fx3 = coeff * (dx1 - inner_coeff3 * dx3);
    real fy3 = coeff * (dy1 - inner_coeff3 * dy3);
    real fz3 = coeff * (dz1 - inner_coeff3 * dz3);
    real fx2 = -(fx1 + fx3);
    real fy2 = -(fy1 + fy3);
    real fz2 = -(fz1 + fz3);

    #pragma omp atomic
    forces[idx1][0] += fx1;
    #pragma omp atomic
    forces[idx2][0] += fx2;
    #pragma omp atomic
    forces[idx3][0] += fx3;
    #pragma omp atomic
    forces[idx1][1] += fy1;
    #pragma omp atomic
    forces[idx2][1] += fy2;
    #pragma omp atomic
    forces[idx3][1] += fy3;
    #pragma omp atomic
    forces[idx1][2] += fz1;
    #pragma omp atomic
    forces[idx2][2] += fz2;
    #pragma omp atomic
    forces[idx3][2] += fz3;
  }
}
