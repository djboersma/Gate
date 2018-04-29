FIND_PACKAGE(Boost 1.54 COMPONENTS unit_test_framework system filesystem)

IF (NOT Boost_FOUND)
MESSAGE(WARNING "BOOST not found the normal way")
SET(Boost_NO_BOOST_CMAKE ON)
FIND_PACKAGE(Boost 1.54)
ENDIF (NOT Boost_FOUND)

IF (Boost_FOUND)
MESSAGE(WARNING "BOOST was found")
ELSE(Boost_FOUND)
MESSAGE(WARNING "BOOST was definitely not found")
ENDIF(Boost_FOUND)

FILE(GLOB GENERAL_TEST_MAIN general/main.cc)
FILE(GLOB GENERAL_TEST_SRCS general/Gate*.cc)
add_executable(general_unit_tests ${GENERAL_TEST_MAIN} ${GENERAL_TEST_SRCS})
add_definitions( -DGATE_SOURCE_DIR=${CMAKE_HOME_DIRECTORY} )

TARGET_LINK_LIBRARIES(general_unit_tests ${ALL_GATE_LINK_LIBRARIES} -L${Boost_LIBRARY_DIRS} -lboost_system -lboost_filesystem)
ADD_TEST(NAME general_unit_tests
         COMMAND general_unit_tests -r detailed)
