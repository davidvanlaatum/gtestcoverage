#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <fstream>
#include <json.hpp>
#include <stdserializers.h>
#include <ClangCoverageData.h>
#include <ClangCoverageFunction.h>
#include <ClangCoverageFile.h>

using namespace testing;
using namespace testing::coverage::clang;
using namespace testing::coverage;

namespace boost {
  namespace filesystem {
    void PrintTo( const boost::filesystem::path &path, std::ostream *os ) {
      *os << path.string();
    }
  }
}

TEST( ClangParserTest, Test ) {
  ClangCoverageDataPtr data;
  {
    std::ifstream file( TEST_LOCATION "/clang2.json", std::ios_base::in );
    nlohmann::json json;
    file >> json;
    data = json;
    std::clog << data;
  }

  auto func = data->getFunction( "_ZN19unittest_Test1_Test8TestBodyEv" );
  ASSERT_NE( func, nullptr );
  EXPECT_EQ( func->getName(), "_ZN19unittest_Test1_Test8TestBodyEv" );
  EXPECT_EQ( func->getHits(), 1 );

  EXPECT_THAT( func->getSources(), SizeIs( 2 ) );

  auto file = data->getFile( "/Users/david/Projects/gtestcoverage/src/GTestCoverageCovers.h" );
  ASSERT_NE( file, nullptr );
  EXPECT_EQ( file->getFilename(), "/Users/david/Projects/gtestcoverage/src/GTestCoverageCovers.h" );
}
