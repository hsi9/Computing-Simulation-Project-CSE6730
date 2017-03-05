#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "H5Cpp.h"

using namespace std;
using namespace H5;

template<typename T, unsigned int N>
vector<array<T, N>> dataset_to_2d_array(const H5File &file, const char *dataset_path, PredType ptype) {
  auto dataset = file.openDataSet(dataset_path);
  auto dataspace = dataset.getSpace();

  auto ndims = dataspace.getSimpleExtentNdims();
  hsize_t dims[ndims];
  dataspace.getSimpleExtentDims(dims, NULL);

  vector<array<T, N>> buffer(dims[0]);
  dataset.read(&buffer[0], ptype);
  return buffer;
}

int main(int argc, char **argv) {
  H5File h5File( "simple.h5", H5F_ACC_RDONLY );
  auto buffer = dataset_to_2d_array<double, 3>(h5File, "topology/bonds/params", PredType::NATIVE_DOUBLE);

  for (const auto &v : buffer) {
    for (const auto &vv : v) {
      cout << vv << " ";
    } cout << "\n";
  } cout << "\n";

  return 0;
}
