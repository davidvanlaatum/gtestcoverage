#pragma once
#ifndef GTESTCOVERAGE_CLANGCOVERAGEFILE_H
#define GTESTCOVERAGE_CLANGCOVERAGEFILE_H

#include "ClangCoverageFwd.h" // IWYU pragma: keep
#include <cstdint>   // for uint32_t
#include <iosfwd>    // for ostream
#include <json.hpp>  // for json
#include "fwd.h"     // for path
#include <stdserializers.h>
#include <filesystemserializers.h>

namespace testing {
  namespace coverage {
    namespace clang {
      class ClangCoverageFileSegment {
      public:
        struct LineAddress {
          uint32_t line;
          uint32_t column;
          bool isLine;
          bool operator<( const LineAddress &rhs ) const;
          bool operator>( const LineAddress &rhs ) const;
          bool operator<=( const LineAddress &rhs ) const;
          bool operator>=( const LineAddress &rhs ) const;
          bool operator==( const LineAddress &rhs ) const;
          bool operator!=( const LineAddress &rhs ) const;
        };
        const LineAddress &getAddress() const;
        bool isLine() const;
        uint32_t getLine() const;
        void merge( const ClangCoverageFileSegment &other );
        ClangCoverageFileSegmentPtr diff( const ClangCoverageFileSegment &other ) const;
        void resetHits();
        uint32_t getHits() const;

      protected:
        LineAddress address;
        uint32_t hits;
        bool hasCount;
        friend void from_json( const nlohmann::json &json, ClangCoverageFileSegment &data );
        friend std::ostream &operator<<( std::ostream &os, const ClangCoverageFileSegment &data );
      };

      void from_json( const nlohmann::json &json, ClangCoverageFileSegment &data );
      std::ostream &operator<<( std::ostream &os, const ClangCoverageFileSegment &data );

      class ClangLineInfo {
      public:
        uint32_t getHits() const;
        void addHits( uint32_t count );
        void resetHits();
      protected:
        uint32_t hits;
      };

      class ClangCoverageFile {
      public:
        const path &getFilename() const;
        void merge( const ClangCoverageFile &other );
        ClangCoverageFilePtr diff( const ClangCoverageFile &other ) const;
        void resetHits();
        void fill( const testing::coverage::TestInfoPtr &test, const testing::coverage::CoverageDataPtr &data ) const;
      protected:
        path filename;
        std::map<uint32_t, ClangLineInfoPtr> lines;
        std::map<ClangCoverageFileSegment::LineAddress, ClangCoverageFileSegmentPtr> segments;
        void resolveLines();
        friend void from_json( const nlohmann::json &json, ClangCoverageFile &data );
      };

      void from_json( const nlohmann::json &json, ClangCoverageFile &data );
    }
  }
}

#endif //GTESTCOVERAGE_CLANGCOVERAGEFILE_H
