#include "BlockInfo.h"
#include "TestInfo.h"
#include "CoverageData.h"
#include <set>
#include <string>

using namespace testing::coverage;

void testing::coverage::to_json( nlohmann::json &json, const BlockInfo &info ) {
  json = info.location;
  json["hits"] = info.hitCount;
  auto &tests = json["tests"];
  tests = nlohmann::json::array();
  for ( const auto &item : info.tests ) {
    tests.push_back( item.first );
  }
}

void testing::coverage::from_json( const nlohmann::json &json, BlockInfo &info ) {
  info.location = json;
  info.hitCount = json.value<decltype( info.hitCount )>( "hits", 0 );
  for ( const auto &item : json.value<std::vector<std::string>>( "tests", {} ) ) {
    info.tests[item];
  }
}

BlockInfo::BlockInfo( const Block &nLocation ) : location( nLocation ) {}

void BlockInfo::addHits( uint32_t count ) {
  if ( count ) {
    hitCount += count;
  }
}

void BlockInfo::addCoveringTest( const TestInfoPtr &test ) {
  tests[test->getFullName()] = test;
}

void BlockInfo::readResolve( const CoverageDataPtr &data ) {
  for ( auto &item : tests ) {
    item.second = data->getTestByFullName( item.first );
  }
}

const Block &BlockInfo::getLocation() const {
  return location;
}

uint32_t BlockInfo::getHits() const {
  return hitCount;
}
