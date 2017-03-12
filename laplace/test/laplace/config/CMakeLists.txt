lap_create_gtest(
  NAME test_mpi_config
  SOURCE_FILES mpi_config_test.cpp
  DEPENDENCY_LIBS ${yamlcpp_LIBRARIES} laplace
)

lap_create_gtest(
  NAME test_t_coupling_config
  SOURCE_FILES t_coupling_config_test.cpp
  DEPENDENCY_LIBS ${yamlcpp_LIBRARIES} laplace
)
