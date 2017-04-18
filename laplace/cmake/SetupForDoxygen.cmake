include(FindDoxygen REQUIRED)

if (NOT DOXYGEN_FOUND)
    message( FATAL_ERROR "${BoldRed}Did not find doxygen installed!${ColourReset}" )
endif()

if (NOT DOXYGEN_DOT_FOUND)
  message( WARNING "
    Graphviz doesn't seem to be installed.
    Doxygen will not be able to generate graphs.
    Consider installing this package.
  " )
endif (NOT DOXYGEN_DOT_FOUND)

function(lap_create_doxygen_target)
  configure_file(docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile @ONLY)

  set(DOC_TARGET_NAME "docs")
  if (TARGET docs)
    set(DOC_TARGET_NAME "${PROJECT_NAME}_docs")
  endif()

  add_custom_target(
    ${DOC_TARGET_NAME} ${TARGET_ALL}
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation using doxygen for ${PROJECT_NAME}" VERBATIM
  )

  set(INSTALL_DOC_DIR ${CMAKE_BINARY_DIR}/docs/${PROJECT_NAME}/html)
  file(MAKE_DIRECTORY ${INSTALL_DOC_DIR}) # needed for install

  install(
    DIRECTORY ${INSTALL_DOC_DIR}
    DESTINATION share/${PROJECT_NAME}/${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    COMPONENT docs
  )
endfunction()
