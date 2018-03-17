#include "LineInfo.h"

using namespace testing::coverage;

void testing::coverage::to_json( nlohmann::json &json, const LineInfo &/*info*/ ) {
  json = nullptr;
}
