#include "laplace/core/mm_system.h"
#include "laplace/hdf5/h5_read.h"
#include <fmt/format.h>

using namespace laplace;
using namespace laplace::hdf5;
using namespace H5;

MmSystem laplace::MmSystem::load_from_file(const H5::H5File &h5File) {
  MmSystem system;
  system.bonds = hdf5::load_struct_array<Bond>(h5File, "topology/bonds");
  system.angles = hdf5::load_struct_array<Angle>(h5File, "topology/angles");
  system.dihedrals = hdf5::load_struct_array<Torsion>(h5File, "topology/dihedrals");
  system.impropers = hdf5::load_struct_array<Torsion>(h5File, "topology/impropers");
  system.nonbonded14s = hdf5::load_struct_array<NonBonded14>(h5File, "topology/nonbonded14s");
  system.atoms.load_topology(h5File);
  // TODO: let load_trajectory() figure out which trajectory snapshot to load (latest by number)
  system.atoms.load_trajectory(h5File, "trajectory/0000/rvf");
  return system;
}

void MmSystem::print_info() const {
  auto header = Bond::header_str();
  auto line = std::string(header.size(), '=');
  fmt::print("\nBONDS:\n{}\n{}\n", header, line);
  for (const auto &x : bonds) {
    fmt::print("{}\n", x.info_str());
  }

  header = Angle::header_str();
  line = std::string(header.size(), '=');
  fmt::print("\nANGLES:\n{}\n{}\n", header, line);
  for (const auto &x : angles) {
    fmt::print("{}\n", x.info_str());
  }

  header = Torsion::header_str();
  line = std::string(header.size(), '=');
  fmt::print("\nDIHEDRALS:\n{}\n{}\n", header, line);
  for (const auto &x : dihedrals) {
    fmt::print("{}\n", x.info_str());
  }
  fmt::print("\nIMPROPERS:\n{}\n{}\n", header, line);
  for (const auto &x : impropers) {
    fmt::print("{}\n", x.info_str());
  }

  header = NonBonded14::header_str();
  line = std::string(header.size(), '=');
  fmt::print("\nNONBONDED14S:\n{}\n{}\n", header, line);
  for (const auto &x : nonbonded14s) {
    fmt::print("{}\n", x.info_str());
  }

  atoms.print_info();
}
