#ifndef LAP_HDF5_H5_TYPE_H
#define LAP_HDF5_H5_TYPE_H

#include <H5Cpp.h>
#include <vector>
#include <tuple>

namespace laplace { namespace hdf5 {
  using H5TypeMapEntry = std::tuple<std::string, hid_t, H5::PredType>;

  template <typename T>
  H5::CompType build_h5_type(const std::vector<H5TypeMapEntry> &typemap) {
    H5::CompType mtype( sizeof(T) );
    for (const auto &t : typemap) {
      mtype.insertMember(
        H5std_string(std::get<0>(t).c_str()),
        std::get<1>(t),
        std::get<2>(t)
      );
    }
    return mtype;
  }
}}

#endif
