find_package(HDF5 REQUIRED)
message( "${HDF5_FOUND} ${HDF5_VERSION} ${HDF5_C_LIBRARIES} ${HDF5_IS_PARALLEL}")
include_directories(${HDF5_INCLUDE_DIRS})
