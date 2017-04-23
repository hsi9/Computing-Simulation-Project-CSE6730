#ifndef LAP_HDF5_H5_READ_H
#define LAP_HDF5_H5_READ_H

#include "laplace/hdf5/h5_type.h"
#include <H5Cpp.h>
#include <vector>
#include <tuple>

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
    auto dataspace = dataset.getSpace();

    // get the number of dimensions
    auto ndims = dataspace.getSimpleExtentNdims();

    // get the size of each dimension
    std::vector<hsize_t> dims(ndims);
    dataspace.getSimpleExtentDims(&dims[0], NULL);

    // set up the dataspace
    hsize_t d_count[1] = { dims[0] };
    hsize_t d_start[1] = { 0 };
    dataspace.selectHyperslab(H5S_SELECT_SET, d_count, d_start);

    // set up the memspace
    DataSpace memspace(1, &dims[0]);
    memspace.selectHyperslab(H5S_SELECT_SET, d_count, d_start);

    // copy the data over using the memspace and dataspace specs
    std::vector<T> buffer(dims[0]);
    dataset.read(&buffer[0], mtype, memspace, dataspace);
    return buffer;
  }

  /*
    Requires that type T be a primitive type
  */
  template<typename T>
  std::vector<T> load_struct_array_column(const H5File &h5File,
                                          const std::string &table,
                                          const std::string &column,
                                          const H5::PredType &ptype) {
    auto dataset = h5File.openDataSet(table);
    auto dataspace = dataset.getSpace();

    // construct temporary type to extract column off of struct array
    H5::CompType mtype(sizeof(T));
    mtype.insertMember(H5std_string(column.c_str()), 0, ptype);

    // get the number of dimensions
    auto ndims = dataspace.getSimpleExtentNdims();

    // get the size of each dimension
    std::vector<hsize_t> dims(ndims);
    dataspace.getSimpleExtentDims(&dims[0], NULL);

    // set up the dataspace
    hsize_t d_count[1] = { dims[0] };
    hsize_t d_start[1] = { 0 };
    dataspace.selectHyperslab(H5S_SELECT_SET, d_count, d_start);

    DataSpace memspace(1, &dims[0]);
    memspace.selectHyperslab(H5S_SELECT_SET, d_count, d_start);

    // read the actual data in
    std::vector<T> buffer(dims[0]);
    dataset.read(&buffer[0], mtype, memspace, dataspace);
    return buffer;
  }

  /*
    Requires that type T be a primitive type
  */
  template<typename T, unsigned int N>
  std::vector<std::array<T, N>> load_2d_array_n_columns(const H5File &h5File,
                                                        const std::string &table,
                                                        const H5::PredType &ptype,
                                                        const hsize_t start_column = 0) {
    auto dataset = h5File.openDataSet(table);
    auto dataspace = dataset.getSpace();

    // get the number of dimensions
    auto ndims = dataspace.getSimpleExtentNdims();

    // get the size of each dimension
    std::vector<hsize_t> dims(ndims);
    dataspace.getSimpleExtentDims(&dims[0], NULL);

    /*
      declare the dataspace - this is a DESCRIPTOR of the source.
      dataspace is a 2D layout buffer and starts at (0, start_column)

      For more information on hyperslabs, please see:
        https://support.hdfgroup.org/HDF5/doc/H5.intro.html#Intro-PMSelectHyper
    */
    hsize_t d_count[2] = { 1, 1 };
    hsize_t d_start[2] = { 0, start_column };
    hsize_t d_block[2] = { dims[0], N };
    dataspace.selectHyperslab(H5S_SELECT_SET, d_count, d_start, NULL, d_block);

    /*
      declare the memspace - this is a DESCRIPTOR of the destination.
      memspace is a 1D layout buffer and starts at 0
    */
    hsize_t m_count[1] = { dims[0] * N };
    hsize_t m_start[1] = { 0 };
    hsize_t m_dims[1] = { dims[0] * N };
    DataSpace memspace(1, m_dims);
    memspace.selectHyperslab(H5S_SELECT_SET, m_count, m_start);

    // copy the data over using the memspace and dataspace specs
    std::vector<std::array<T, N>> buffer(dims[0]);
    dataset.read(&buffer[0], ptype, memspace, dataspace);
    return buffer;
  }
}}

#endif
