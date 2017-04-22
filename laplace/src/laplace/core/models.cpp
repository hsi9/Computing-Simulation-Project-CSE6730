#include "laplace/core/models.h"
#include "laplace/hdf5/h5_type.h"

using namespace laplace;
using namespace laplace::hdf5;

/*
  NOTE: Code should choose between H5::PredType::NATIVE_DOUBLE and
  H5::PredType::NATIVE_FLOAT depending on what the typedef "real" is referring to
*/

H5::CompType laplace::Bond::h5_type() {
  return hdf5::build_h5_type<Bond>({{
    hdf5::H5TypeMapEntry { "i", HOFFSET(Bond, ids[0]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "j", HOFFSET(Bond, ids[1]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "r0", HOFFSET(Bond, r0), H5::PredType::NATIVE_DOUBLE },
    hdf5::H5TypeMapEntry { "konst", HOFFSET(Bond, konst), H5::PredType::NATIVE_DOUBLE }
  }});
}

H5::CompType laplace::Angle::h5_type() {
  return hdf5::build_h5_type<Angle>({{
    hdf5::H5TypeMapEntry { "i", HOFFSET(Angle, ids[0]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "j", HOFFSET(Angle, ids[1]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "k", HOFFSET(Angle, ids[2]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "theta0", HOFFSET(Angle, theta0), H5::PredType::NATIVE_DOUBLE },
    hdf5::H5TypeMapEntry { "konst", HOFFSET(Angle, konst), H5::PredType::NATIVE_DOUBLE }
  }});
}

H5::CompType laplace::Torsion::h5_type() {
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

H5::CompType laplace::NonBonded14::h5_type() {
  return hdf5::build_h5_type<NonBonded14>({{
    hdf5::H5TypeMapEntry { "i", HOFFSET(NonBonded14, ids[0]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "j", HOFFSET(NonBonded14, ids[1]), H5::PredType::NATIVE_INT },
    hdf5::H5TypeMapEntry { "coeff_vdw", HOFFSET(NonBonded14, coeff_vdw), H5::PredType::NATIVE_DOUBLE },
    hdf5::H5TypeMapEntry { "coeff_ele", HOFFSET(NonBonded14, coeff_ele), H5::PredType::NATIVE_DOUBLE }
  }});
}
