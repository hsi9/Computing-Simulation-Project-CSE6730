#include "laplace/hdf5/h5_util.h"

using namespace std;
namespace hdf5 = laplace::hdf5;

herr_t collect_subgroups(hid_t loc_id, const char *name, void *opdata) {
  auto vecptr = static_cast<vector<string> *>(opdata);
  /*
   * Get type of the object and display its name and type.
   * The name of the object is passed to this function by
   * the Library. Some magic :-)
   */
  H5G_stat_t statbuf;
  H5Gget_objinfo(loc_id, name, false, &statbuf);
  switch (statbuf.type) {
    case H5G_GROUP:
      vecptr->emplace_back(name);
      break;
    default:
       ;
  }
  return 0;
}

vector<string> hdf5::list_subgroups(H5::H5File &h5File,
                                    const string &path) {
  vector<string> subgroups;
  h5File.iterateElems(path, NULL, collect_subgroups, static_cast<void *>(&subgroups));
  return subgroups;
}
