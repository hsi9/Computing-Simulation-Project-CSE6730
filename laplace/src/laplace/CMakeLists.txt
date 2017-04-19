configure_file(laplacepre.h.cmakein laplacepre.h @ONLY)

set(LIBLAPLACE_SOURCES)

add_subdirectory(utils)
add_subdirectory(config)
add_subdirectory(math)
add_subdirectory(mpi)
add_subdirectory(core)

add_library(laplace ${LIBLAPLACE_SOURCES})

target_link_libraries(laplace
    ${MPI_C_LIBRARIES}
    ${MPI_CXX_LIBRARIES}
    ${fmtlib_LIBRARIES}
    ${yamlcpp_LIBRARIES}
    ${docoptcpp_LIBRARIES})

add_dependencies(laplace
    fmtlib
    yamlcpp
    docoptcpp)

lap_add_install_targets(TARGETS laplace)
