#pragma once
#ifndef GTESTCOVERAGE_BLOCKINFO_H
#define GTESTCOVERAGE_BLOCKINFO_H

#include "fwd.h" // IWYU pragma: keep
#include "Block.h"
#include <memory>
#include <json.hpp>             // for json
#include <cstdint>

namespace testing {
  namespace coverage {
    class BlockInfo : public std::enable_shared_from_this<BlockInfo> {
    public:
      BlockInfo() = default;
      BlockInfo( const Block &nLocation );
      void addHits( uint32_t count, const std::string &test );
      const Block &getLocation() const;
      uint32_t getHits() const;
    protected:
      Block location;
      uint32_t hitCount{ 0 };
      std::map<std::string, uint32_t> tests;

      friend void to_json( nlohmann::json &json, const BlockInfo &info );
      friend void from_json( const nlohmann::json &json, BlockInfo &info );
    };
    void to_json( nlohmann::json &json, const BlockInfo &info );
    void from_json( const nlohmann::json &json, BlockInfo &info );
  }
}

#endif //GTESTCOVERAGE_BLOCKINFO_H
