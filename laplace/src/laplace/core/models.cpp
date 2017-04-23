#include "laplace/core/models.h"
#include "laplace/hdf5/h5_type.h"
#include "laplace/hdf5/h5_read.h"
#include <fmt/format.h>

using namespace std;
using namespace laplace;
using namespace laplace::hdf5;

/*
  NOTE: Code should choose between H5::PredType::NATIVE_DOUBLE and
  H5::PredType::NATIVE_FLOAT depending on what the typedef "real" is referring to
*/

H5::CompType Bond::h5_type() {
  return hdf5::build_h5_type<Bond>({{
    hdf5::H5TypeMapEntry { "i", HOFFSET(Bond, ids[0]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "j", HOFFSET(Bond, ids[1]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "r0", HOFFSET(Bond, r0), H5::PredType::NATIVE_DOUBLE },
    hdf5::H5TypeMapEntry { "konst", HOFFSET(Bond, konst), H5::PredType::NATIVE_DOUBLE }
  }});
}

H5::CompType Angle::h5_type() {
  return hdf5::build_h5_type<Angle>({{
    hdf5::H5TypeMapEntry { "i", HOFFSET(Angle, ids[0]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "j", HOFFSET(Angle, ids[1]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "k", HOFFSET(Angle, ids[2]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "theta0", HOFFSET(Angle, theta0), H5::PredType::NATIVE_DOUBLE },
    hdf5::H5TypeMapEntry { "konst", HOFFSET(Angle, konst), H5::PredType::NATIVE_DOUBLE }
  }});
}

H5::CompType Torsion::h5_type() {
  return hdf5::build_h5_type<Torsion>({{
    hdf5::H5TypeMapEntry { "i", HOFFSET(Torsion, ids[0]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "j", HOFFSET(Torsion, ids[1]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "k", HOFFSET(Torsion, ids[2]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "l", HOFFSET(Torsion, ids[3]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "multiplicity", HOFFSET(Torsion, multiplicity), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "phi0", HOFFSET(Torsion, phi0), H5::PredType::NATIVE_DOUBLE },
    hdf5::H5TypeMapEntry { "konst", HOFFSET(Torsion, konst), H5::PredType::NATIVE_DOUBLE }
  }});
}

H5::CompType NonBonded14::h5_type() {
  return hdf5::build_h5_type<NonBonded14>({{
    hdf5::H5TypeMapEntry { "i", HOFFSET(NonBonded14, ids[0]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "j", HOFFSET(NonBonded14, ids[1]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "coeff_vdw", HOFFSET(NonBonded14, coeff_vdw), H5::PredType::NATIVE_DOUBLE },
    hdf5::H5TypeMapEntry { "coeff_ele", HOFFSET(NonBonded14, coeff_ele), H5::PredType::NATIVE_DOUBLE }
  }});
}

void Atoms::load_topology(const H5::H5File &h5File) {
  masses = hdf5::load_struct_array_column<real>(h5File, "topology/atoms", "mass", H5::PredType::NATIVE_DOUBLE);
  sigma = hdf5::load_struct_array_column<real>(h5File, "topology/atoms", "sigma", H5::PredType::NATIVE_DOUBLE);
  epsilon = hdf5::load_struct_array_column<real>(h5File, "topology/atoms", "epsilon", H5::PredType::NATIVE_DOUBLE);
  q = hdf5::load_struct_array_column<real>(h5File, "topology/atoms", "q", H5::PredType::NATIVE_DOUBLE);
}

void Atoms::load_trajectory(const H5::H5File &h5File,
                                     const string &dataset) {
  positions = hdf5::load_2d_array_n_columns<real, 3>(h5File, dataset, H5::PredType::NATIVE_DOUBLE, 0);
  velocities = hdf5::load_2d_array_n_columns<real, 3>(h5File, dataset, H5::PredType::NATIVE_DOUBLE, 3);
  forces = hdf5::load_2d_array_n_columns<real, 3>(h5File, dataset, H5::PredType::NATIVE_DOUBLE, 6);
  forces_old = forces;
}


string Bond::header_str() {
  return fmt::format("{:>13}{:>13}{:>13}{:>13}", "i", "j", "konst", "r0");
}

string Bond::info_str() const {
  return fmt::format("{:> 13d}{:> 13d}{:> 13.5g}{:> 13.5g}", ids[0], ids[1], konst, r0);
}

string Angle::header_str() {
  return fmt::format("{:>13}{:>13}{:>13}{:>13}{:>13}", "i", "j", "k", "konst", "theta0");
}

string Angle::info_str() const {
  return fmt::format("{:> 13d}{:> 13d}{:> 13d}{:> 13.5g}{:> 13.5g}", ids[0], ids[1], ids[2], konst, theta0);
}

string Torsion::header_str() {
  return fmt::format("{:>13}{:>13}{:>13}{:>13}{:>13}{:>13}{:>13}", "i", "j", "k", "l", "konst", "phi0", "mult");
}

string Torsion::info_str() const {
  return fmt::format("{:> 13d}{:> 13d}{:> 13d}{:> 13d}{:> 13.5g}{:> 13.5g}{:> 13d}", ids[0], ids[1], ids[2], ids[3], konst, phi0, multiplicity);
}

string NonBonded14::header_str() {
  return fmt::format("{:>13}{:>13}{:>13}{:>13}", "i", "j", "coeff_vdw", "coeff_ele");
}

string NonBonded14::info_str() const {
  return fmt::format("{:> 13d}{:> 13d}{:> 13.5g}{:> 13.5g}", ids[0], ids[1], coeff_vdw, coeff_ele);
}

void Atoms::print_info() const {
  auto header = fmt::format("{:>13}{:>13}{:>13}{:>13}{:>13}", "i", "mass", "sigma", "epsilon", "q");
  auto line = string(header.size(), '=');

  fmt::print("\nATOMS:\n{}\n{}\n", header, line);
  for (auto i=0U; i < positions.size(); i++) {
    fmt::print("{:> 13d}{:> 13.5g}{:> 13.5g}{:> 13.5g}{:> 13.5g}\n", (int)i, masses[i], sigma[i], epsilon[i], q[i]);
  }

  header = fmt::format("{:>13}{:>13}{:>13}{:>13}{:>13}{:>13}{:>13}{:>13}{:>13}{:>13}", "i", "Rx", "Ry", "Rz", "Vx", "Vy", "Vz", "Fx", "Fy", "Fz");
  line = string(header.size(), '=');

  fmt::print("\nTRAJECTORY:\n{}\n{}\n", header, line);
  for (auto i=0U; i < positions.size(); i++) {
    fmt::print(
      "{:> 13d}{:> 13.5g}{:> 13.5g}{:> 13.5g}{:> 13.5g}{:> 13.5g}{:> 13.5g}{:> 13.5g}{:> 13.5g}{:> 13.5g}\n",
      (int)i,
      positions[i][0], positions[i][1], positions[i][2],
      velocities[i][0], velocities[i][1], velocities[i][2],
      forces[i][0], forces[i][1], forces[i][2]
    );
  }
}
