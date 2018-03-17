#pragma once
#ifndef GTESTCOVERAGE_BLOCKINFO_H
#define GTESTCOVERAGE_BLOCKINFO_H

#include "fwd.h"
#include "Block.h"
#include <memory>
#include <json.hpp>             // for json
#include <cstdint>

namespace testing {
  namespace coverage {
    class BlockInfo : public std::enable_shared_from_this<BlockInfo> {
    public:
      BlockInfo( const Block &nLocation );
      void addHits( uint32_t count );
    protected:
      Block location;
      uint32_t hitCount{ 0 };

      friend void to_json( nlohmann::json &json, const BlockInfo &info );
    };
    void to_json( nlohmann::json &json, const BlockInfo &info );
  }
}

#endif //GTESTCOVERAGE_BLOCKINFO_H
