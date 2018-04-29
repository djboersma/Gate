cmake_minimum_required(VERSION 3.0)

project(cmake_catch_test)

# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make HU builder test executable
add_executable(HU_builder_tests ${CMAKE_CURRENT_SOURCE_DIR}/catch/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/general/GateHounsfieldToMaterialBuilderTest.cc)
add_definitions( -DGATE_SOURCE_DIR=${CMAKE_HOME_DIRECTORY} )
target_link_libraries(HU_builder_tests Catch GateImpl ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${EXTRA_GATE_LINK_LIBRARIES} )
ADD_TEST(NAME HU_builder_tests
         COMMAND HU_builder_tests -d yes)

# Make HU density table test executable
add_executable(HU_density_table_tests ${CMAKE_CURRENT_SOURCE_DIR}/catch/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/general/GateHounsfieldDensityTableTest.cc)
target_link_libraries(HU_density_table_tests Catch GateImpl ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${EXTRA_GATE_LINK_LIBRARIES} )
ADD_TEST(NAME HU_density_table_tests
         COMMAND HU_density_table_tests -d yes)
