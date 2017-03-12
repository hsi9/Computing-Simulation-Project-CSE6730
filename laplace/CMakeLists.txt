message(STATUS "CMake version: ${CMAKE_VERSION}")
cmake_minimum_required( VERSION 3.6.0 )

# Determine if fmt is built as a subproject (using add_subdirectory)
# or if it is the master project.
set(MASTER_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MASTER_PROJECT ON)
endif()

option(LAPLACE_PEDANTIC "Enable extra warnings and expensive tests." OFF)
option(LAPLACE_DOCS "Generate the docs target." OFF)
option(LAPLACE_INSTALL "Generate the install target." ${MASTER_PROJECT})
option(LAPLACE_TESTS "Generate the test target." OFF)
# option(BUILD_SHARED_LIBS "Build as shared libraries" OFF)

# CMake modules/macros are in a subdirectory to keep this file cleaner
# This needs to be set before PROJECT() in order to pick up toolchain files
list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
include(SetGitProjectInformation)
project( laplace VERSION ${VERSION_STRING_GENERATED_FROM_GIT_REPO_INFO} )
set(PROJECT_DESCIPTION "A Molecular Dynamics Package")

if ("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    MESSAGE(FATAL_ERROR "\nERROR! ${PROJECT_NAME} DOES NOT SUPPORT IN SOURCE BUILDS!\n"
      "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
      " == CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}\n"
      "NEXT STEPS:\n"
      "(1) Delete the CMakeCache.txt file and the CMakeFiles/ directory\n"
      "     under the source directory for ${PROJECT_NAME}, otherwise you\n"
      "     will not be able to configure ${PROJECT_NAME} correctly!\n"
      "      * For example, on linux machines do:\n"
      "        $ rm -r CMakeCache.txt CMakeFiles/\n"
      "(2) Create a different directory and configure ${PROJECT_NAME} in that directory.\n"
      "      * For example, on linux machines do:\n"
      "        $ mkdir target\n"
      "        $ cd target/\n"
      "        $ cmake [OPTIONS] ..\n"
      )
endif()

# Set the output binaries to be located in <target>/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(SetCMakeMessageColors)

message("
    ${BoldCyan}**** WELCOME TO ${CMAKE_PROJECT_NAME} (v${PROJECT_VERSION})! ****

    For the list of ${CMAKE_PROJECT_NAME} build options,
    run CMake with the '-LH' flags.${ColourReset}
")

# include(SetBuildInformation)
include(CheckCXXCompiler)
include(SetupInstallTarget)
include(InstallExternalLibraries)

#[[
IF(LAPLACE_GPU OR LAPLACE_GPUECP)
    include(CheckCUDA)
endif()
]]

include(CheckOpenMP)
include(CheckMPI)
include(CheckHDF5)

# Make src/ a search path for include files
include_directories(src)
# Also include the binary tree for CONFIGURE_FILE()-generated header files
include_directories(${CMAKE_BINARY_DIR}/src)
# Build project in the following subdirectories
add_subdirectory(src)

if (LAPLACE_TESTS)
    include(SetupForTests)
endif()

if (LAPLACE_DOCS)
    include(SetupForDoxygen)
    include_directories(docs)
    include_directories(${CMAKE_BINARY_DIR}/docs)
    lap_create_doxygen_target()
endif()

include(CPackConfig)
