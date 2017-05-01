#include "laplace/core/dihedral.h"
#include <cmath>

using namespace std;

// TODO: Shorten wall of calculations.
void laplace::dihedral_forces_update(MmSystem &system,
                                     const real L[3]) {
  auto &positions = system.atoms.positions;
  auto &dihedrals = system.dihedrals;
  auto &forces = system.atoms.forces;

  #pragma omp parallel for
  for (auto i = 0U; i < dihedrals.size(); ++i) {
    auto &dihedral = dihedrals[i];

    auto idx1 = dihedral.ids[0];
    auto idx2 = dihedral.ids[1];
    auto idx3 = dihedral.ids[2];
    auto idx4 = dihedral.ids[3];

    // r1 points from idx1 to idx2
    // r2 points from idx2 to idx3
    // r3 points from idx3 to idx4
    real dx1 = remainder(positions[idx2][0] - positions[idx1][0], L[0]);
    real dy1 = remainder(positions[idx2][1] - positions[idx1][1], L[1]);
    real dz1 = remainder(positions[idx2][2] - positions[idx1][2], L[2]);
    real dx2 = remainder(positions[idx3][0] - positions[idx2][0], L[0]);
    real dy2 = remainder(positions[idx3][1] - positions[idx2][1], L[1]);
    real dz2 = remainder(positions[idx3][2] - positions[idx2][2], L[2]);
    real dx3 = remainder(positions[idx4][0] - positions[idx3][0], L[0]);
    real dy3 = remainder(positions[idx4][1] - positions[idx3][1], L[1]);
    real dz3 = remainder(positions[idx4][2] - positions[idx3][2], L[2]);
    real r1sq = dx1 * dx1 + dy1 * dy1 + dz1 * dz1;
    real r2sq = dx2 * dx2 + dy2 * dy2 + dz2 * dz2;
    real r3sq = dx3 * dx3 + dy3 * dy3 + dz3 * dz3;

    real mag_r2 = sqrt(r2sq);

    // m is r1 cross r2
    // n is r2 cross r3
    real mx = dy1 * dz2 - dy2 * dz1;
    real my = dz1 * dx2 - dz2 * dx1;
    real mz = dx1 * dy2 - dx2 * dy1;
    real nx = dy2 * dz3 - dy3 * dz2;
    real ny = dz2 * dx3 - dz3 * dx2;
    real nz = dx2 * dy3 - dx3 * dy2;

    real m_cross_n_x = my * nz - ny * mz;
    real m_cross_n_y = mz * nx - nz * mx;
    real m_cross_n_z = mx * ny - nx * my;

    real m_dot_n = mx * nx + my * ny + mz * nz; 

    // (m cross n) dot r2
    real sin_phi_nonnormalized = m_cross_n_x * dx2 + m_cross_n_y * dy2 + m_cross_n_z * dz2;
    // (m dot n) * mag_r2
    real cos_phi_nonnormalized = m_dot_n * mag_r2;
    real tan_phi = sin_phi_nonnormalized / cos_phi_nonnormalized;
    // IUPAC standard phi has flipped signs
    real phi = - atan(tan_phi);

    real coeff = dihedral.multiplicity 
        * dihedral.konst
        * sin(dihedral.multiplicity * phi - dihedral.phi0)
        / (1.0 + tan_phi * tan_phi);

    // m cross r1, r2, r3
    real m_cross_r1_x = my * dz1 - dy1 * mz;
    real m_cross_r1_y = mz * dx1 - dz1 * mx;
    real m_cross_r1_z = mx * dy1 - dx1 * my;
    real m_cross_r2_x = my * dz2 - dy2 * mz;
    real m_cross_r2_y = mz * dx2 - dz2 * mx;
    real m_cross_r2_z = mx * dy2 - dx2 * my;
    real m_cross_r3_x = my * dz3 - dy3 * mz;
    real m_cross_r3_y = mz * dx3 - dz3 * mx;
    real m_cross_r3_z = mx * dy3 - dx3 * my;

    // n cross r1, r2, r3
    real n_cross_r1_x = ny * dz1 - dy1 * nz;
    real n_cross_r1_y = nz * dx1 - dz1 * nx;
    real n_cross_r1_z = nx * dy1 - dx1 * ny;
    real n_cross_r2_x = ny * dz2 - dy2 * nz;
    real n_cross_r2_y = nz * dx2 - dz2 * nx;
    real n_cross_r2_z = nx * dy2 - dx2 * ny;
    real n_cross_r3_x = ny * dz3 - dy3 * nz;
    real n_cross_r3_y = nz * dx3 - dz3 * nx;
    real n_cross_r3_z = nx * dy3 - dx3 * ny;

    // "upper" part of f (nabla on numerator)
    // coefficient: 1 / ((m dot n) * mag_r2)
    real upper_coeff = 1.0 / cos_phi_nonnormalized;
    // nabla ((m cross n) dot r2)
    // (n cross r2) cross r2
    real upper_fx1 = (n_cross_r2_y * dz2 - dy2 * n_cross_r2_z) * upper_coeff;
    real upper_fy1 = (n_cross_r2_z * dx2 - dz2 * n_cross_r2_x) * upper_coeff;
    real upper_fz1 = (n_cross_r2_x * dy2 - dx2 * n_cross_r2_y) * upper_coeff;
    // (n cross m) - ((m cross r2) cross r3 + (n cross r2) cross (r1 + r2))
    real upper_fx2 = (- m_cross_n_x - ((m_cross_r2_y * dz3 - dy3 * m_cross_r2_z) + (n_cross_r2_y * (dz1 + dz2) - (dy1 + dy2) * n_cross_r2_z))) * upper_coeff;
    real upper_fy2 = (- m_cross_n_y - ((m_cross_r2_z * dx3 - dz3 * m_cross_r2_x) + (n_cross_r2_z * (dx1 + dx2) - (dz1 + dz2) * n_cross_r2_x))) * upper_coeff;
    real upper_fz2 = (- m_cross_n_z - ((m_cross_r2_x * dy3 - dx3 * m_cross_r2_y) + (n_cross_r2_x * (dy1 + dy2) - (dx1 + dx2) * n_cross_r2_y))) * upper_coeff;
    // (m cross n) + ((n cross r2) cross r1 + (m cross r2) cross (r2 + r3))
    real upper_fx3 = (m_cross_n_x + ((n_cross_r2_y * dz1 - dy1 * n_cross_r2_z) + (m_cross_r2_y * (dz2 + dz3) - (dy2 + dy3) * m_cross_r2_z))) * upper_coeff;
    real upper_fy3 = (m_cross_n_y + ((n_cross_r2_z * dx1 - dz1 * n_cross_r2_x) + (m_cross_r2_z * (dx2 + dx3) - (dz2 + dz3) * m_cross_r2_x))) * upper_coeff;
    real upper_fz3 = (m_cross_n_z + ((n_cross_r2_x * dy1 - dx1 * n_cross_r2_y) + (m_cross_r2_x * (dy2 + dy3) - (dx2 + dx3) * m_cross_r2_y))) * upper_coeff;
    // (r2 cross m) cross r2
    real upper_fx4 = - (m_cross_r2_y * dz2 - dy2 * m_cross_r2_z) * upper_coeff;
    real upper_fy4 = - (m_cross_r2_z * dx2 - dz2 * m_cross_r2_x) * upper_coeff;
    real upper_fz4 = - (m_cross_r2_x * dy2 - dx2 * m_cross_r2_y) * upper_coeff;

    // "lower" part of f (nabla on denominator)
    // coefficient: ((m cross n) dot r2) / ((m dot n) * mag_r2))^2
    real lower_coeff = - tan_phi / cos_phi_nonnormalized;
    // nabla ((m dot n) * mag_r2) * -((m cross n) dot r2) / ((m dot n) * mag_r2)^2
    // mag_r2 * (n cross r2)
    real lower_fx1 = (mag_r2 * n_cross_r2_x) * lower_coeff;
    real lower_fy1 = (mag_r2 * n_cross_r2_y) * lower_coeff;
    real lower_fz1 = (mag_r2 * n_cross_r2_z) * lower_coeff;
    // - (m dot n) * (r2 / mag_r2) + ((r1 cross n) + (r2 cross n) + (m cross r3))
    real lower_fx2 = (- m_dot_n * dx2 / mag_r2 + mag_r2 * (- n_cross_r1_x - n_cross_r2_x + m_cross_r3_x)) * lower_coeff;
    real lower_fy2 = (- m_dot_n * dy2 / mag_r2 + mag_r2 * (- n_cross_r1_y - n_cross_r2_y + m_cross_r3_y)) * lower_coeff;
    real lower_fz2 = (- m_dot_n * dz2 / mag_r2 + mag_r2 * (- n_cross_r1_z - n_cross_r2_z + m_cross_r3_z)) * lower_coeff;
    // (m dot n) * (r2 / mag_r2) + ((n cross r1) + (r2 cross m) + (r3 cross m))
    real lower_fx3 = (m_dot_n * dx2 / mag_r2 + mag_r2 * (n_cross_r1_x - m_cross_r2_x - m_cross_r3_x)) * lower_coeff;
    real lower_fy3 = (m_dot_n * dy2 / mag_r2 + mag_r2 * (n_cross_r1_y - m_cross_r2_y - m_cross_r3_y)) * lower_coeff;
    real lower_fz3 = (m_dot_n * dz2 / mag_r2 + mag_r2 * (n_cross_r1_z - m_cross_r2_z - m_cross_r3_z)) * lower_coeff;
    // mag_r2 * (m cross r2)
    real lower_fx4 = (mag_r2 * m_cross_r2_x);
    real lower_fy4 = (mag_r2 * m_cross_r2_y);
    real lower_fz4 = (mag_r2 * m_cross_r2_z);

    // cosine multi-welled dihedral force
    real fx1 = coeff * (upper_coeff * upper_fx1 + lower_coeff * lower_fx1);
    real fy1 = coeff * (upper_coeff * upper_fy1 + lower_coeff * lower_fy1);
    real fz1 = coeff * (upper_coeff * upper_fz1 + lower_coeff * lower_fz1);
    real fx2 = coeff * (upper_coeff * upper_fx2 + lower_coeff * lower_fx2);
    real fy2 = coeff * (upper_coeff * upper_fy2 + lower_coeff * lower_fy2);
    real fz2 = coeff * (upper_coeff * upper_fz2 + lower_coeff * lower_fz2);
    real fx3 = coeff * (upper_coeff * upper_fx3 + lower_coeff * lower_fx3);
    real fy3 = coeff * (upper_coeff * upper_fy3 + lower_coeff * lower_fy3);
    real fz3 = coeff * (upper_coeff * upper_fz3 + lower_coeff * lower_fz3);
    real fx4 = coeff * (upper_coeff * upper_fx4 + lower_coeff * lower_fx4);
    real fy4 = coeff * (upper_coeff * upper_fy4 + lower_coeff * lower_fy4);
    real fz4 = coeff * (upper_coeff * upper_fz4 + lower_coeff * lower_fz4);

    #pragma omp atomic
    forces[idx1][0] += fx1;
    #pragma omp atomic
    forces[idx2][0] += fx2;
    #pragma omp atomic
    forces[idx3][0] += fx3;
    #pragma omp atomic
    forces[idx4][0] += fx4;
    #pragma omp atomic
    forces[idx1][1] += fy1;
    #pragma omp atomic
    forces[idx2][1] += fy2;
    #pragma omp atomic
    forces[idx3][1] += fy3;
    #pragma omp atomic
    forces[idx4][1] += fy4;
    #pragma omp atomic
    forces[idx1][2] += fz1;
    #pragma omp atomic
    forces[idx2][2] += fz2;
    #pragma omp atomic
    forces[idx3][2] += fz3;
    #pragma omp atomic
    forces[idx4][2] += fz4;
  }
}
