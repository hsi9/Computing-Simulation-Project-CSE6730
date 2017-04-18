enable_testing()

include(InstallGTest)
find_package(GTest REQUIRED)

# define this function first before including the test subdirectory
# because CMake scripts are evaluated in order
function(lap_create_gtest)
    cmake_parse_arguments(
      CREATE_GTEST_ARGS              # prefix of output variables
      ""                             # list of names of the boolean arguments (only defined ones will be true)
      "NAME"                         # list of names of mono-valued arguments
      "SOURCE_FILES;DEPENDENCY_LIBS" # list of names of multi-valued arguments (output variables are lists)
      ${ARGN}                        # arguments of the function to parse, here we take the all original ones
    )

    set(PROJECT_TEST_NAME ${CMAKE_PROJECT_NAME}_${CREATE_GTEST_ARGS_NAME})

    file(GLOB TEST_SRC_FILES ${CREATE_GTEST_ARGS_SOURCE_FILES})
    add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})

    if (NOT WIN32 OR MINGW)
        target_link_libraries(
          ${PROJECT_TEST_NAME}
          ${GTEST_LIBS_DIR}/libgtest.a
          ${GTEST_LIBS_DIR}/libgtest_main.a
        )
    else()
        target_link_libraries(
          ${PROJECT_TEST_NAME}
          debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
          optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
        target_link_libraries(
          ${PROJECT_TEST_NAME}
          debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
          optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
    endif()

    set_target_properties(
      ${PROJECT_TEST_NAME}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests
    )
    target_link_libraries(${PROJECT_TEST_NAME}
                          ${CMAKE_THREAD_LIBS_INIT}
                          ${fmtlib_LIBRARIES}
                          ${CREATE_GTEST_ARGS_DEPENDENCY_LIBS})
    add_dependencies(${PROJECT_TEST_NAME} googletest)
    add_test(${CREATE_GTEST_ARGS_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/${PROJECT_TEST_NAME})
    set_property(TEST ${CREATE_GTEST_ARGS_NAME} PROPERTY ENVIRONMENT "GTEST_COLOR=1")
endfunction()

include_directories(${GTEST_INCLUDE_DIR})
add_subdirectory(test)
