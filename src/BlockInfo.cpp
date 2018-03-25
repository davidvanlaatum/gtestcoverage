#include "BlockInfo.h"
#include "TestInfo.h"
#include <set>
#include <string>

using namespace testing::coverage;

void testing::coverage::to_json( nlohmann::json &json, const BlockInfo &info ) {
  json = info.location;
  json["hits"] = info.hitCount;
  json["tests"] = info.tests;
}

void testing::coverage::from_json( const nlohmann::json &json, BlockInfo &info ) {
  info.location = json;
  info.hitCount = json.value<decltype( info.hitCount )>( "hits", 0 );
  info.tests = json.value<decltype( info.tests )>( "tests", {} );
}

BlockInfo::BlockInfo( const Block &nLocation ) : location( nLocation ) {}

void BlockInfo::addHits( uint32_t count, const std::string &test ) {
  if ( count ) {
    hitCount += count;
    tests[test] += count;
  }
}

const Block &BlockInfo::getLocation() const {
  return location;
}

uint32_t BlockInfo::getHits() const {
  return hitCount;
}
