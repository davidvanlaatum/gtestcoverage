#include "BlockInfo.h"

using namespace testing::coverage;

void testing::coverage::to_json( nlohmann::json &json, const BlockInfo &info ) {
  json = info.location;
  json["hits"] = info.hitCount;
}

BlockInfo::BlockInfo( const Block &nLocation ) : location( nLocation ) {}

void BlockInfo::addHits( uint32_t count ) {
  hitCount += count;
}
