cmake_minimum_required(VERSION 3.0)

project(cmake_catch_test)

# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make test executable
add_executable(HU_builder_tests ${CMAKE_CURRENT_SOURCE_DIR}/catch/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/general/GateHounsfieldToMaterialBuilderTest.cc)
add_definitions( -DGATE_SOURCE_DIR=${CMAKE_HOME_DIRECTORY} )
target_link_libraries(HU_builder_tests Catch GateImpl ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${CLHEP_LIBRARIES} ${LIBXML2_LIBRARIES} ${LMF_LIBRARY} ${ECAT7_LIBRARY} ${RTK_LIBRARIES} ${ITK_LIBRARIES} )
