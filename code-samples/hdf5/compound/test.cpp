#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "H5Cpp.h"

using namespace std;
using namespace H5;

struct Bond {
  // DELIBERATELY REMOVE THIS FIELD
  // int64_t gid;
  int64_t  i, j;
  double c0, c1, c2;

  void print() const {
    cout /*<< gid << " "*/ << i << " " << j << " " << c0 << " " << c1 << " " << c2 << "\n";
  }
};

int main(int argc, char **argv) {
  const int LENGTH = 10;

  H5File h5File( "compound.h5", H5F_ACC_RDONLY );
  auto dataset = h5File.openDataSet("topology/bonds");

  CompType mtype( sizeof(Bond) );
  // DELIBERATELY REMOVE THIS FIELD
  // mtype.insertMember( H5std_string("gid"), HOFFSET(Bond, gid), PredType::NATIVE_INT64);
  mtype.insertMember( H5std_string("i"), HOFFSET(Bond, i), PredType::NATIVE_INT64);
  mtype.insertMember( H5std_string("j"), HOFFSET(Bond, j), PredType::NATIVE_INT64);
  mtype.insertMember( H5std_string("alpha"), HOFFSET(Bond, c0), PredType::NATIVE_DOUBLE);
  mtype.insertMember( H5std_string("beta"), HOFFSET(Bond, c1), PredType::NATIVE_DOUBLE);
  mtype.insertMember( H5std_string("gamma"), HOFFSET(Bond, c2), PredType::NATIVE_DOUBLE);

  // init the dataspaces
  auto dataspace = dataset.getSpace();
  hsize_t dimsm[1] = { LENGTH };
  DataSpace memspace( 1, dimsm );

  // get the dimensions of the
  auto ndims = dataspace.getSimpleExtentNdims();
  hsize_t dims[ndims];
  dataspace.getSimpleExtentDims(dims, NULL);
  cout << "dims: " << "(";
  for (int i=0; i < ndims; i++) {
    cout << dims[i] << ",";
  } cout << ")\n\n";


  // set the dataspace portions to copy and paste into
  hsize_t offset_out[1] = { 0 };
  hsize_t count_out[1] = { LENGTH };
  memspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );
  dataspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );

  // read the actual data in
  vector<Bond> bonds(LENGTH);
  dataset.read( &bonds[0], mtype, memspace, dataspace );

  for (const auto &b : bonds) {
    b.print();
  } cout << "\n";

  return 0;
}
