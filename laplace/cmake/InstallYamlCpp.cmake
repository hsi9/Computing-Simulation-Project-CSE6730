ExternalProject_Add(
  yamlcpp
  GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp.git"
  GIT_TAG "master"
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  PREFIX "${CMAKE_BINARY_DIR}/external/yamlcpp"
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/yamlcpp"
  CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} -DCMAKE_BUILD_TYPE=Release -DYAML_CPP_BUILD_TOOLS=0 -DYAML_CPP_BUILD_CONTRIB=0
  TEST_COMMAND ""
  INSTALL_COMMAND cmake -E echo "Skipping install step."
)

ExternalProject_Add_Step(
  yamlcpp CopyToBin
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/external/yamlcpp/bin ${GLOBAL_OUTPUT_PATH}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/external/yamlcpp/lib ${GLOBAL_OUTPUT_PATH}
  DEPENDEES install
)

ExternalProject_Get_Property(yamlcpp source_dir)
ExternalProject_Get_Property(yamlcpp binary_dir)

set(yamlcpp_INCLUDE_DIRS "${source_dir}/include")

if (BUILD_SHARED_LIBS)
  set(yamlcpp_LIBRARIES "${binary_dir}/${CMAKE_SHARED_LIBRARY_PREFIX}yaml-cpp${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
  set(yamlcpp_LIBRARIES "${binary_dir}/${CMAKE_STATIC_LIBRARY_PREFIX}yaml-cpp${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

include_directories(${yamlcpp_INCLUDE_DIRS})
