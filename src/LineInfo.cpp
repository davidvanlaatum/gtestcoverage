#include "LineInfo.h"
#include "TestInfo.h"
#include "CoverageData.h"

using namespace testing::coverage;

void testing::coverage::to_json( nlohmann::json &json, const LineInfo &info ) {
  json["hits"] = info.hits;
  auto &tests = json["tests"];
  tests = nlohmann::json::array();
  for ( const auto &item : info.tests ) {
    tests.push_back( item.first );
  }
}

void testing::coverage::from_json( const nlohmann::json &json, LineInfo &info ) {
  info.hits = json.value<uint32_t>( "hits", 0 );
  for ( const auto &item : json.value<std::vector<std::string>>( "tests", {} ) ) {
    info.tests[item];
  }
}

size_t LineInfo::getHits() const {
  return hits;
}

void LineInfo::addHits( uint32_t count ) {
  hits += count;
}

void LineInfo::addCoveringTest( const TestInfoPtr &test ) {
  tests[test->getFullName()] = test;
}

void LineInfo::readResolve( const CoverageDataPtr &data ) {
  for ( auto &item : tests ) {
    item.second = data->getTestByFullName( item.first );
  }
}

bool LineInfo::hasCoverage() const {
  return hits > 0;
}
