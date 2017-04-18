ExternalProject_Add(
  docoptcpp
  GIT_REPOSITORY "https://github.com/docopt/docopt.cpp.git"
  GIT_TAG "v0.6.2"
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  PREFIX "${CMAKE_BINARY_DIR}/external/docoptcpp"
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/docoptcpp"
  CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
  TEST_COMMAND ""
  INSTALL_COMMAND cmake -E echo "Skipping install step."
)

ExternalProject_Get_Property(docoptcpp binary_dir)

set(docoptcpp_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external")

if (BUILD_SHARED_LIBS)
  set(docoptcpp_LIBRARIES "${binary_dir}/${CMAKE_SHARED_LIBRARY_PREFIX}docopt${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
  set(docoptcpp_LIBRARIES "${binary_dir}/${CMAKE_STATIC_LIBRARY_PREFIX}docopt${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

include_directories(${docoptcpp_INCLUDE_DIRS})
