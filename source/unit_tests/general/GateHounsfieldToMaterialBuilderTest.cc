#include <boost/test/unit_test.hpp>
#include <GateHounsfieldToMaterialsBuilder.hh>

BOOST_AUTO_TEST_SUITE( HounsfieldToMaterialTests )

BOOST_AUTO_TEST_CASE( hello )
{
    BOOST_MESSAGE( "Hello world!" );
}

BOOST_AUTO_TEST_CASE( instantiate )
{
    GateHounsfieldToMaterialsBuilder* fh2mb = new GateHounsfieldToMaterialsBuilder;
}

BOOST_AUTO_TEST_SUITE_END()
