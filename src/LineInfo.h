#pragma once
#ifndef GTESTCOVERAGE_LINEINFO_H
#define GTESTCOVERAGE_LINEINFO_H

#include "fwd.h"     // for FileInfoWeakPtr
#include <cstdint>   // for uint32_t
#include <json.hpp>  // for json
#include <memory>    // for enable_shared_from_this

namespace testing {
  namespace coverage {
    class LineInfo : public std::enable_shared_from_this<LineInfo> {
    public:
      LineInfo() = default;

      LineInfo( const FileInfoPtr &filePtr, uint32_t number ) : file( filePtr ), lineNumber( number ) {}

      void addHits( uint32_t count );
      void addCoveringTest( const TestInfoPtr &test );
      void readResolve( const CoverageDataPtr &data );
      bool hasCoverage() const;
      size_t getHits() const;
    protected:
      FileInfoWeakPtr file;
      uint32_t lineNumber{ 0 };
      size_t hits{ 0 };
      friend void to_json( nlohmann::json &json, const LineInfo &info );
      friend void from_json( const nlohmann::json &json, LineInfo &info );
    };
    void to_json( nlohmann::json &json, const LineInfo &info );
    void from_json( const nlohmann::json &json, LineInfo &info );
  }
}

#endif //GTESTCOVERAGE_LINEINFO_H
