#include "laplace/core/mm_system.h"
#include "laplace/hdf5/h5_read.h"
#include <H5Cpp.h>

using namespace laplace;
using namespace laplace::hdf5;
using namespace H5;

MmSystem laplace::MmSystem::load_from_file(const std::string &filepath) {
  H5::H5File h5File(filepath, H5F_ACC_RDONLY);
  MmSystem system;
  system.bonds = hdf5::load_struct_array<Bond>(h5File, "topology/bonds");
  system.angles = hdf5::load_struct_array<Angle>(h5File, "topology/angles");
  system.dihedrals = hdf5::load_struct_array<Torsion>(h5File, "topology/dihedrals");
  system.impropers = hdf5::load_struct_array<Torsion>(h5File, "topology/impropers");
  system.nonbonded14s = hdf5::load_struct_array<NonBonded14>(h5File, "topology/nonbonded14");
  return system;
}
