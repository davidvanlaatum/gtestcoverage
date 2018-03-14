#include "TestInfo.h"
#include <filesystemserializers.h>

using namespace testing::coverage;

TestInfo::TestInfo( const std::string &nName, const TestCaseInfoPtr &pTestCase ) : name( nName ), testCase( pTestCase ) {}

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

//void testing::coverage::from_json( const nlohmann::json &j, TestInfo &data ) {
//}

void testing::coverage::to_json( nlohmann::json &j, const TestInfo &data ) {
  j["success"] = data.success;
  j["coveredFiles"] = data.coveredFiles;
  j["coveredFunctions"] = data.coveredFunctions;
}

void TestInfo::addCoveredFile( const boost::filesystem::path &file ) {
  coveredFiles.emplace( file );
}

void TestInfo::addCoveredFunction( const std::string &functionName ) {
  coveredFunctions.emplace( functionName );
}

const std::set<boost::filesystem::path> &TestInfo::getCoveredFiles() const {
  return coveredFiles;
}

const std::set<std::string> &TestInfo::getCoveredFunctions() const {
  return coveredFunctions;
}
