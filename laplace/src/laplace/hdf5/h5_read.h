#ifndef LAP_HDF5_H5_READ_H
#define LAP_HDF5_H5_READ_H

#include <H5Cpp.h>
#include <vector>

using namespace H5;

namespace laplace { namespace hdf5 {
  /*
    Requires that type T is default constructible and has the static method h5_type() defined
  */
  template<typename T>
  std::vector<T> load_struct_array(const H5File &h5File,
                                      const std::string &table) {
    H5::CompType mtype = T::h5_type();
    auto dataset = h5File.openDataSet(table);

    // init and get the dimensions of the dataspaces
    auto dataspace = dataset.getSpace();
    auto ndims = dataspace.getSimpleExtentNdims();
    hsize_t dims[ndims];
    dataspace.getSimpleExtentDims(dims, NULL);
    DataSpace memspace( 1, dims );

    // set up the dataspace, i.e. the windows to copy from and pasting into
    hsize_t offset_out[1] = { 0 };
    hsize_t count_out[1] = { dims[0] };
    memspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );
    dataspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );

    // read the actual data in
    std::vector<T> items;
    items.resize(dims[0]);
    dataset.read( &items[0], mtype, memspace, dataspace );

    return items;
  }
}}

#endif
