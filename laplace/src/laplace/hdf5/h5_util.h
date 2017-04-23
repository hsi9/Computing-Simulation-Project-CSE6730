#ifndef LAP_HDF5_H5_UTIL_H
#define LAP_HDF5_H5_UTIL_H

#include <H5Cpp.h>
#include <vector>

using namespace H5;

namespace laplace { namespace hdf5 {
  std::vector<std::string> list_subgroups(H5::H5File &h5File,
                                          const std::string &path);
}}

#endif
