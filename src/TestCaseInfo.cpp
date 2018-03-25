#include "TestCaseInfo.h"
#include "TestInfo.h"       // for TestInfo
#include <stdserializers.h> // IWYU pragma: keep
#include <utility>          // for pair

using namespace testing::coverage;

TestCaseInfo::TestCaseInfo( const std::string &nName ) : name( nName ) {}

TestInfoPtr TestCaseInfo::getTest( const std::string &testName ) {
  auto &rt = tests[testName];
  if ( not rt ) {
    rt = std::make_shared<TestInfo>( testName, shared_from_this() );
  }
  return rt;
}

TestInfoPtr TestCaseInfo::findTest( const std::string &testName ) {
  TestInfoPtr rt;
  auto item = tests.find( testName );
  if ( item != tests.end() ) {
    rt = item->second;
  }
  return rt;
}

const std::string &TestCaseInfo::getName() const {
  return name;
}

void TestCaseInfo::setName( const std::string &nName ) {
  name = nName;
}

void testing::coverage::from_json( const nlohmann::json &j, testing::coverage::TestCaseInfo &data ) {
  data.tests = j.get<decltype( data.tests )>();
  for ( const auto &item : data.tests ) {
    item.second->setName( item.first );
    item.second->setTestCase( data.shared_from_this() );
  }
}

void testing::coverage::to_json( nlohmann::json &j, const testing::coverage::TestCaseInfo &data ) {
  j = data.tests;
}
