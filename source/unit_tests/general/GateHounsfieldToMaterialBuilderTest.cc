#include <boost/test/unit_test.hpp>
#include <GateHounsfieldToMaterialsBuilder.hh>
#include <string>
#include <vector>
#include <fstream>

// TO DO: find a portable and safe way to generate temporary filenames
// tmpnam/tmpfile is deprecated
// mkstemp is not portable
// boost::filesystem::unique_path requires boost with libraries, not only headers
// TO DO: maybe force user to provide link to GateContrib, or other path for standard input data

struct HUfixture {
    HUfixture() : hu2mb(new GateHounsfieldToMaterialsBuilder),
                  tmp_material_file("tmp_material_table.txt"),
                  tmp_density_file("tmp_density_table.txt"),
                  tmp_output_db("tmp_output_db.txt"),
                  tmp_output_table("tmp_output_table.txt"){
        BOOST_TEST_MESSAGE( "Doing: setup of HU fixture" );
        std::ofstream fmaterials(tmp_material_file);
        // copy from GateContrib/dosimetry/Radiotherapy/example2/data/SimpleMaterialsTable.txt
        fmaterials
            << "# ===============================================================================" << std::endl
            << "[Elements] " << std::endl
            << "Hydrogen  Carbon Nitrogen Oxygen Sodium Magnesium Phosphor Sulfur " << std::endl
            << "Chlorine Argon Potassium Calcium " << std::endl
            << "Titanium Copper Zinc  Silver Tin  " << std::endl
            << "[/Elements]" << std::endl
            << "# ===============================================================================" << std::endl
            << "# HU      H    C    N    O   Na  Mg   P   S   Cl  Ar  K   Ca  Ti  Cu Zn  Ag  Sn  " << std::endl
            << "# ===============================================================================" << std::endl
            << " -1050    0    0  75.5 23.2  0   0    0   0   0  1.3  0    0  0    0  0  0   0  Air" << std::endl
            << "  -950  10.3 10.5  3.1 74.9 0.2  0   0.2 0.3 0.3  0  0.2   0  0    0  0  0   0  Lung" << std::endl
            << "   19   10.3 13.4  3.0 72.3 0.2  0   0.2 0.2 0.2  0  0.2   0  0    0  0  0   0  SoftTissue" << std::endl
            << "  200    5.6 26.5  3.6 40.5 0.1 0.2  7.3 0.3  0   0   0  15.9 0    0  0  0   0  Bone" << std::endl
            << "  1640   0   0     0   0    0   0   0    0    0   0   0    0  0    4  2  65  29  AmalgamTooth" << std::endl
            << "  2300   0   0     0   0    0   0   0    0    0   0   0    0  100  0  0  0   0  MetallImplants" << std::endl
            << "  3000   0   0     0   0    0   0   0    0    0   0   0    0  100  0  0  0   0  MetallImplants" << std::endl;
        std::ofstream fdensities(tmp_density_file);
        // copy from GateContrib/dosimetry/Radiotherapy/example2/data/Schneider2000DensitiesTable.txt
        fdensities
            << "# ===================" << std::endl
            << "# HU\tdensity g/cm3" << std::endl
            << "# ===================" << std::endl
            << "-1000\t1.21e-3" << std::endl
            << "-98\t0.93" << std::endl
            << "-97\t0.930486" << std::endl
            << "14\t1.03" << std::endl
            << "23\t1.031" << std::endl
            << "100\t1.119900" << std::endl
            << "101\t1.076200" << std::endl
            << "1600\t1.964200" << std::endl
            << "3000\t2.8" << std::endl;

        hu2mb->SetMaterialTable(tmp_material_file);
        hu2mb->SetDensityTable(tmp_material_file);
        hu2mb->SetOutputMaterialDatabaseFilename(tmp_output_db);
        hu2mb->SetOutputHUMaterialFilename(tmp_output_table);
        hu2mb->SetDensityTolerance( 0.1 );
        BOOST_TEST_MESSAGE( "Done: setup of HU fixture" );
    }
    ~HUfixture(){
        BOOST_TEST_MESSAGE( "Doing: teardown of HU fixture" );
/*
        for (auto &s : std::vector<std::string>{tmp_material_file,tmp_density_file,tmp_output_db,tmp_output_table}){
            if ( std::remove(s.c_str()) == 0 ){
                BOOST_TEST_MESSAGE( std::string("deleted: ") + s );
            } else {
                BOOST_TEST_MESSAGE( std::string("not found: ") + s );
            }
        }
*/
        delete hu2mb;
        BOOST_TEST_MESSAGE( "Done: teardown of HU fixture" );
    }
    // objects created for each test
    GateHounsfieldToMaterialsBuilder* hu2mb;
    std::string tmp_material_file;
    std::string tmp_density_file;
    std::string tmp_output_db;
    std::string tmp_output_table;
};

/******************************************************************************************************************/

BOOST_FIXTURE_TEST_SUITE( HounsfieldToMaterialTests, HUfixture )

BOOST_AUTO_TEST_CASE( construct_destruct )
{
    BOOST_TEST_MESSAGE( "Constructing table builder, not building any tables, destruct...");
    BOOST_TEST_WARN( "Constructing table builder, not building any tables, destruct...");
}

BOOST_AUTO_TEST_CASE( normal_build )
{
    BOOST_TEST_MESSAGE( "build standard simple table without crashing" );
    BOOST_TEST_WARN( "build standard simple table without crashing" );
    hu2mb->BuildAndWriteMaterials();
}

BOOST_AUTO_TEST_SUITE_END()
