#include "TestCaseInfo.h"
#include "TestInfo.h"  // for TestInfo
#include <utility>     // for pair
#include <stdserializers.h> // IWYU pragma: keep

using namespace testing::coverage;

TestCaseInfo::TestCaseInfo( const std::string &nName ) : name( nName ) {}

TestInfoPtr TestCaseInfo::getTest( const std::string &testName ) {
  auto &rt = tests[testName];
  if ( not rt ) {
    rt = std::make_shared<TestInfo>( testName, shared_from_this() );
  }
  return rt;
}

const std::string &TestCaseInfo::getName() const {
  return name;
}

//void testing::coverage::from_json( const nlohmann::json &j, testing::coverage::TestCaseInfo &data ) {
//}

void testing::coverage::to_json( nlohmann::json &j, const testing::coverage::TestCaseInfo &data ) {
  for ( const auto &test : data.tests ) {
    j.emplace( test.first, test.second );
  }
}
