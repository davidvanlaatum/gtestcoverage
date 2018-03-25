#pragma once
#ifndef GTESTCOVERAGE_CLANGCOVERAGEFUNCTION_H
#define GTESTCOVERAGE_CLANGCOVERAGEFUNCTION_H

#include "fwd.h"               // for path, CoverageDataPtr, FunctionInfoPtr, TestInfoPtr
#include "ClangCoverageFwd.h"  // for ClangCoverageFunctionSegmentPtr, ClangCoverageFunctionPtr
#include "Block.h"             // for BlockWithFilename
#include <cstdint>             // for uint32_t
#include <iosfwd>              // for ostream
#include <json.hpp>            // for json
#include <map>                 // for map
#include <list>                // for list
#include <memory>              // for enable_shared_from_this
#include <set>                 // for set
#include <string>              // for string

namespace testing {
  namespace coverage {
    namespace clang {
      class ClangCoverageFunctionSegment : public std::enable_shared_from_this<ClangCoverageFunctionSegment> {
      public:
        enum RegionKind {
          /// A CodeRegion associates some code with a counter
              CodeRegion,
          /// An ExpansionRegion represents a file expansion region that associates
          /// a source range with the expansion of a virtual source file, such as
          /// for a macro instantiation or #include file.
              ExpansionRegion,
          /// A SkippedRegion represents a source range with code that was skipped
          /// by a preprocessor or similar means.
              SkippedRegion,
          /// A GapRegion is like a CodeRegion, but its count is only set as the
          /// line execution count when its the only region in the line.
              GapRegion,
          /// Default unknown kind
              Unknown
        };
        struct ClangCoverageFunctionSegmentId : BlockWithFilename {
          RegionKind kind{ Unknown };
          uint32_t fileId{ 0 };
          uint32_t expandedFileId{ 0 };
          ClangCoverageFunctionSegmentId();
          ClangCoverageFunctionSegmentId( const Point &nStart, const Point &nEnd );
          bool operator==( const ClangCoverageFunctionSegmentId &rhs ) const;
          bool operator!=( const ClangCoverageFunctionSegmentId &rhs ) const;
          bool operator<( const ClangCoverageFunctionSegmentId &rhs ) const;
          bool operator>( const ClangCoverageFunctionSegmentId &rhs ) const;
          bool operator<=( const ClangCoverageFunctionSegmentId &rhs ) const;
          bool operator>=( const ClangCoverageFunctionSegmentId &rhs ) const;
        };
        typedef ClangCoverageFunctionSegmentId locationType;
        ClangCoverageFunctionSegment();
        explicit ClangCoverageFunctionSegment( const locationType &nLocation );
        bool operator==( const ClangCoverageFunctionSegment &rhs ) const;
        bool operator!=( const ClangCoverageFunctionSegment &rhs ) const;
        bool contains( const ClangCoverageFunctionSegment &other ) const;
        const locationType &getLocation() const;
        uint32_t getHitCount() const;
        const path &getFile() const;
        uint32_t getFileIndex() const;
        void merge( const ClangCoverageFunctionSegment &other );
        void addHits( uint32_t count );
        void resetHits();
        ClangCoverageFunctionSegmentPtr diff( const ClangCoverageFunctionSegment &other ) const;
      protected:
        locationType location;
        uint32_t hitCount{ 0 };
        uint32_t fileIndex{ 0 };
        friend void from_json( const nlohmann::json &json, ClangCoverageFunctionSegment &data );
        friend void from_json( const nlohmann::json &json, ClangCoverageFunction &data );
        friend std::ostream &operator<<( std::ostream &os, const ClangCoverageFunctionSegment &data );
      };

      void from_json( const nlohmann::json &json, ClangCoverageFunctionSegment &data );
      std::ostream &operator<<( std::ostream &os, const ClangCoverageFunctionSegment &data );
      std::ostream &operator<<( std::ostream &os, ClangCoverageFunctionSegment::RegionKind kind );

      class ClangCoverageFunction {
      public:
        const std::string &getName() const;
        void merge( const ClangCoverageFunction &other );
        ClangCoverageFunctionPtr diff( const ClangCoverageFunction &other ) const;
        const std::set<path> &getSources() const;
        uint32_t getHits() const;
        void resetHits();
        void fill( const FunctionInfoPtr &function ) const;
        void fill( const testing::coverage::TestInfoPtr &test, const testing::coverage::CoverageDataPtr &data ) const;
        std::vector<ClangCoverageFunctionSegmentPtr> getSegmentsByFileOrder() const;
      protected:
        std::string name;
        std::map<ClangCoverageFunctionSegment::locationType, ClangCoverageFunctionSegmentPtr> segments;
        std::set<path> sources;
        uint32_t hits{ 0 };
        friend void from_json( const nlohmann::json &json, ClangCoverageFunction &data );
        friend std::ostream &operator<<( std::ostream &os, const ClangCoverageFunction &data );
      };

      void from_json( const nlohmann::json &json, ClangCoverageFunction &data );
      std::ostream &operator<<( std::ostream &os, const ClangCoverageFunction &data );
    }
  }
}

#endif //GTESTCOVERAGE_CLANGCOVERAGEFUNCTION_H
