# General CPack configuration
# Info: http://www.itk.org/Wiki/CMake:Component_Install_With_CPack
# _____________________________________________________________________________

if (APPLE)
  set(CPACK_PACKAGE_VENDOR "The Celeste Group") # PackageMaker doesn't like http://
else()
  # deb lintian insists on URL
  set(CPACK_PACKAGE_VENDOR "http://www.protein.osaka-u.ac.jp/rcsfp/pi/omegagene")
endif()

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCIPTION})
set(CPACK_PACKAGE_CONTACT "Benson Ma <bm@berkeley.edu>")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK ${PROJECT_VERSION_TWEAK})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_STRIP_FILES TRUE)

if (MSVC)
  set(CPACK_GENERATOR "NSIS")
endif()

if (APPLE)
  set(CPACK_GENERATOR "PackageMaker")
  set(CPACK_SET_DESTDIR ON)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES Linux)
  FIND_PROGRAM(RPM_EXE rpmbuild)
  if (${RPM_EXE} MATCHES RPM_EXE-NOTFOUND)
    set(CPACK_GENERATOR "TGZ;DEB")
  else()
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
  endif()
endif()

set(CPACK_NSIS_DISPLAY_NAME ${CMAKE_PROJECT_NAME})

# Components (Disabled)
set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)

# RPM specific configuration (minimum)
set(CPACK_RPM_PACKAGE_LICENSE "Apache License 2.0")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Engineering")
set(CPACK_RPM_COMPONENT_INSTALL ON)   # necessary even if CPACK_COMPONENT_INSTALL set to ON
set(CPACK_RPM_PACKAGE_DEBUG ON)

set(CPACK_DEBIAN_PACKAGE_DEBUG ON)

include(InstallRequiredSystemLibraries)
include(CPack)
