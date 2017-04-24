#include "laplace/hdf5/h5_util.h"
#include "laplace/utils/string.h"
#include <fmt/format.h>

namespace hdf5 = laplace::hdf5;
namespace stringutil = laplace::util::string;
using namespace std;

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

vector<string> hdf5::list_subgroups(const H5::H5File &h5File,
                                    const string &path) {
  vector<string> subgroups;
  const_cast<H5::H5File &>(h5File).iterateElems(path, NULL, collect_subgroups, static_cast<void *>(&subgroups));
  return subgroups;
}

void hdf5::create_subgroup(const H5::H5File &h5File,
                           const std::string &path) {
  // disable logging errors here because we will be expecting non-existent paths
  H5Eset_auto(0, NULL, NULL);

  string tmppath;
  for (const auto &component : stringutil::split(path, '/')) {
    tmppath += fmt::format("/{}", component);
    if (H5Gget_objinfo(h5File.getId(), tmppath.c_str(), 0, NULL) != 0) {
      h5File.createGroup(tmppath);
    }
  }
}
