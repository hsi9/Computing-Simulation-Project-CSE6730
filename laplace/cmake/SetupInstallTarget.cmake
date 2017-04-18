# Setup for make install
function(lap_add_install_targets)
  cmake_parse_arguments(
    CREATE_INSTALL_ARGS # prefix of output variables
    ""                  # list of names of the boolean arguments (only defined ones will be true)
    ""                  # list of names of mono-valued arguments
    "TARGETS"           # list of names of multi-valued arguments (output variables are lists)
    ${ARGN}             # arguments of the function to parse, here we take the all original ones
  )

  # binaries will be installed to bin/
  # libraries will be installed to lib/
  # docs will be installed to share/
  install(
    TARGETS ${CREATE_INSTALL_ARGS_TARGETS}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib/${PROJECT_NAME}/${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    ARCHIVE DESTINATION lib/${PROJECT_NAME}/${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}/static
  )
endfunction()

# Setup for make uninstall
# remove all the files plus the encapsulating lib/<project>/<version> subdirectory
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/UninstallTarget.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/UninstallTarget.cmake
    @ONLY
)
add_custom_target(
    uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/UninstallTarget.cmake
)
