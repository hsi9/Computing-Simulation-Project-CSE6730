set(ExternalDependencyLibs
    ${MPI_C_LIBRARIES}
    ${MPI_CXX_LIBRARIES}
    ${fmtlib_LIBRARIES}
    ${yamlcpp_LIBRARIES}
    ${docoptcpp_LIBRARIES})

add_executable(la+ la+.cpp)
target_link_libraries(la+ laplace ${ExternalDependencyLibs})

lap_add_install_targets(TARGETS la+)
