#pragma once
#ifndef GTESTCOVERAGE_CLANGCOVERAGEFUNCTION_H
#define GTESTCOVERAGE_CLANGCOVERAGEFUNCTION_H

#include <string>
#include <json.hpp>
#include <iosfwd>
#include <set>
#include <boost/filesystem/path.hpp>
#include <memory>
#include <boost/fusion/container/set.hpp>
#include "ClangCoverageFwd.h"

namespace testing {
  namespace coverage {
    namespace clang {
      class ClangCoverageFunction;
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
              GapRegion
        };
        struct Point {
          uint32_t line = 0;
          uint32_t column = 0;
          bool operator==( const Point &rhs ) const;
          bool operator!=( const Point &rhs ) const;
          bool operator<( const Point &rhs ) const;
          bool operator>( const Point &rhs ) const;
          bool operator<=( const Point &rhs ) const;
          bool operator>=( const Point &rhs ) const;
        };
        bool operator==( const ClangCoverageFunctionSegment &rhs ) const;
        bool operator!=( const ClangCoverageFunctionSegment &rhs ) const;
        bool contains( const ClangCoverageFunctionSegment &other ) const;
      protected:
        Point start, end;
        uint32_t hitCount;
        boost::filesystem::path file;
        RegionKind kind;
        uint32_t fileId;
        uint32_t expandedFileId;
        friend void from_json( const nlohmann::json &json, ClangCoverageFunctionSegment &data );
        friend void from_json( const nlohmann::json &json, ClangCoverageFunction &data );
        friend std::ostream &operator<<( std::ostream &os, const ClangCoverageFunctionSegment &data );
      };

      void from_json( const nlohmann::json &json, ClangCoverageFunctionSegment &data );
      std::ostream &operator<<( std::ostream &os, const ClangCoverageFunctionSegment &data );
      std::ostream &operator<<( std::ostream &os, ClangCoverageFunctionSegment::RegionKind kind );
      std::ostream &operator<<( std::ostream &os, const ClangCoverageFunctionSegment::Point &point );

      class ClangCoverageFunction {
      public:
        const std::string &getName() const;
        void merge( const ClangCoverageFunction &other );
        ClangCoverageFunctionPtr diff( const ClangCoverageFunction &other ) const;
        const std::set<boost::filesystem::path> &getSources() const;
        uint32_t getHits() const;
        void fill( const FunctionInfoPtr &function ) const;
      protected:
        std::string name;
        std::vector<ClangCoverageFunctionSegment> segments;
        std::set<boost::filesystem::path> sources;
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
