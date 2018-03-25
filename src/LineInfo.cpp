#include "LineInfo.h"

using namespace testing::coverage;

void testing::coverage::to_json( nlohmann::json &json, const LineInfo &info ) {
  json["hits"] = info.hits;
}

void testing::coverage::from_json( const nlohmann::json &json, LineInfo &info ) {
  info.hits = json.value<uint32_t>( "hits", 0 );
}

size_t LineInfo::getHits() const {
  return hits;
}

void LineInfo::addHits( uint32_t count ) {
  hits += count;
}

void LineInfo::addCoveringTest( const TestInfoPtr &/*test*/ ) {
}

void LineInfo::readResolve( const CoverageDataPtr &/*data*/ ) {
}

bool LineInfo::hasCoverage() const {
  return hits > 0;
}
