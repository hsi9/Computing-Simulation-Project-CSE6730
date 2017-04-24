#ifndef LAP_HDF5_H5_WRITE_H
#define LAP_HDF5_H5_WRITE_H

#include "laplace/hdf5/h5_type.h"
#include <H5Cpp.h>
#include <vector>

using namespace H5;

namespace laplace { namespace hdf5 {
  H5::DataSet create_2d_array(H5File &h5File,
                              const std::string &table,
                              const hsize_t rows,
                              const hsize_t cols,
                              const H5::PredType &ptype) {
      hsize_t dims[2];
      dims[0] = rows;
      dims[1] = cols;
      DataSpace dataspace(2, dims);
      return h5File.createDataSet(table, ptype, dataspace);
  }

  /*
    Requires that type T be a primitive type
  */
  template<typename T, unsigned int N>
  void write_2d_array_n_columns(H5::DataSet &dataset,
                                const std::vector<std::array<T, N>> &data,
                                const H5::PredType &ptype,
                                const hsize_t start_column = 0) {
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
    hsize_t m_count[1] = { data.size() * N };
    hsize_t m_start[1] = { 0 };
    hsize_t m_dims[1] = { data.size() * N };
    DataSpace memspace(1, m_dims);
    memspace.selectHyperslab(H5S_SELECT_SET, m_count, m_start);

    // copy the data over using the memspace and dataspace specs
    dataset.write(&data[0], ptype, memspace, dataspace);
    return;
  }
}}

#endif
