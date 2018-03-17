#pragma once
#ifndef GTESTCOVERAGE_LINEINFO_H
#define GTESTCOVERAGE_LINEINFO_H

#include "fwd.h"
#include <memory>
#include <json.hpp>

namespace testing {
  namespace coverage {
    class LineInfo : public std::enable_shared_from_this<LineInfo> {
    public:

    protected:
      FileInfoWeakPtr file;
      uint32_t lineNumber{ 0 };
      friend void to_json( nlohmann::json &json, const LineInfo &info );
    };
    void to_json( nlohmann::json &json, const LineInfo &info );
  }
}

#endif //GTESTCOVERAGE_LINEINFO_H
