set(GTEST_FORCE_SHARED_CRT ON)
set(GTEST_DISABLE_PTHREADS OFF)

# Google Test is broken for MinGW (see https://github.com/google/googletest/issues/606)
# Work around by adding -Dgtest_disable_pthreads=ON (see https://github.com/google/shaderc/pull/174)
if (MINGW)
    set(GTEST_DISABLE_PTHREADS ON)
endif()

ExternalProject_Add(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG "release-1.8.0"
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  PREFIX "${CMAKE_BINARY_DIR}/external/googletest"
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/googletest"
  CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
  -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
  -Dgtest_force_shared_crt=${GTEST_FORCE_SHARED_CRT}
  -Dgtest_disable_pthreads=${GTEST_DISABLE_PTHREADS}
  -DBUILD_GTEST=ON # -DCMAKE_BUILD_TYPE=Release
  INSTALL_COMMAND cmake -E echo "Skipping install step."
  # write log to file instead of STDOUT
  # LOG_DOWNLOAD ON
  # LOG_CONFIGURE ON
  # LOG_BUILD ON
)

ExternalProject_Get_Property(googletest source_dir)
ExternalProject_Get_Property(googletest binary_dir)

# Specify the variables needed by find_package(GTest)
set(GTEST_INCLUDE_DIR ${source_dir}/googletest/include)
set(GTEST_LIBS_DIR ${binary_dir}/googlemock/gtest)
set(GTEST_LIBRARY gtest)
set(GTEST_MAIN_LIBRARY ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_STATIC_LIBRARY_SUFFIX})
