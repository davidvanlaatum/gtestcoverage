#include "TestInfo.h"
#include "TestCaseInfo.h"
#include <filesystemserializers.h> // IWYU pragma: keep

using namespace testing::coverage;

TestInfo::TestInfo( const std::string &nName, const TestCaseInfoPtr &pTestCase ) : name( nName ), testCase( pTestCase ) {}

TestInfo::TestInfo( std::string &&nName, const TestCaseInfoPtr &pTestCase ) : name( std::move( nName ) ), testCase( pTestCase ) {}

bool TestInfo::isSuccess() const {
  return success;
}

void TestInfo::setSuccess( bool nSuccess ) {
  success = nSuccess;
}

const std::string &TestInfo::getName() const {
  return name;
}

TestCaseInfoPtr TestInfo::getTestCase() const {
  return testCase.lock();
}

void testing::coverage::from_json( const nlohmann::json &j, TestInfo &data ) {
  data.success = j.value( "success", false );
  data.coveredFiles = j.value<decltype( data.coveredFiles )>( "coveredFiles", {} );
  data.coveredFunctions = j.value<decltype( data.coveredFunctions )>( "coveredFunctions", {} );
}

void testing::coverage::to_json( nlohmann::json &j, const TestInfo &data ) {
  j["success"] = data.success;
  j["coveredFiles"] = data.coveredFiles;
  j["coveredFunctions"] = data.coveredFunctions;
}

void TestInfo::addCoveredFile( const path &file ) {
  coveredFiles.emplace( file );
}

void TestInfo::addCoveredFunction( const std::string &functionName ) {
  coveredFunctions.emplace( functionName );
}

const std::set<path> &TestInfo::getCoveredFiles() const {
  return coveredFiles;
}

const std::set<std::string> &TestInfo::getCoveredFunctions() const {
  return coveredFunctions;
}

void TestInfo::setName( const std::string &nName ) {
  name = nName;
}

void TestInfo::setTestCase( const TestCaseInfoPtr &pTestCase ) {
  testCase = pTestCase;
}

std::string TestInfo::getFullName() const {
  auto parent = testCase.lock();
  return ( parent ? parent->getName() : std::string( "unknown" ) ) + "::" + getName();
}
