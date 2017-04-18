ExternalProject_Add(
  fmtlib
  GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
  GIT_TAG "3.0.1"
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  PREFIX "${CMAKE_BINARY_DIR}/external/fmtlib"
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/fmtlib"
  CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} -DFMT_TEST=0 -DCMAKE_BUILD_TYPE=Release
  TEST_COMMAND ""
  INSTALL_COMMAND cmake -E echo "Skipping install step."
)

ExternalProject_Get_Property(fmtlib source_dir)
ExternalProject_Get_Property(fmtlib binary_dir)

set(fmtlib_INCLUDE_DIRS ${source_dir})
if (BUILD_SHARED_LIBS)
  set(fmtlib_LIBRARIES "${binary_dir}/fmt/${CMAKE_SHARED_LIBRARY_PREFIX}fmt${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
  set(fmtlib_LIBRARIES "${binary_dir}/fmt/${CMAKE_STATIC_LIBRARY_PREFIX}fmt${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

include_directories(${fmtlib_INCLUDE_DIRS})
