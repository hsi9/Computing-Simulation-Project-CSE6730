#ifndef LAP_HDF5_H5_UTIL_H
#define LAP_HDF5_H5_UTIL_H

#include <H5Cpp.h>
#include <vector>

using namespace H5;

namespace laplace { namespace hdf5 {
  std::vector<std::string> list_subgroups(const H5::H5File &h5File,
                                          const std::string &path);

  /*
	Create a subgroup in a manner similar to `mkdir -p`.
	HDF5 does not contain a method to create intermediate paths
	for a nested group if the intermediate path does not exist.
  */
  void create_subgroup(const H5::H5File &h5File,
                       const std::string &path);
}}

#endif
