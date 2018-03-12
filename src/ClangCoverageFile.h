#pragma once
#ifndef GTESTCOVERAGE_CLANGCOVERAGEFILE_H
#define GTESTCOVERAGE_CLANGCOVERAGEFILE_H

#include <json.hpp>
#include <iosfwd>
#include <boost/filesystem/path.hpp>

namespace testing {
  namespace coverage {
    namespace clang {
      class ClangCoverageFileSegment {
      public:
      protected:
        uint32_t line;
        uint32_t column;
        uint32_t hits;
        bool isLine;
        bool hasCount;
        friend void from_json( const nlohmann::json &json, ClangCoverageFileSegment &data );
        friend std::ostream &operator<<( std::ostream &os, const ClangCoverageFileSegment &data );
      };

      void from_json( const nlohmann::json &json, ClangCoverageFileSegment &data );
      std::ostream &operator<<( std::ostream &os, const ClangCoverageFileSegment &data );

      class ClangCoverageFile {
      public:
        const boost::filesystem::path &getFilename() const;
        void merge( const ClangCoverageFile &other );
      protected:
        boost::filesystem::path filename;
        friend void from_json( const nlohmann::json &json, ClangCoverageFile &data );
      };

      void from_json( const nlohmann::json &json, ClangCoverageFile &data );
    }
  }
}

#endif //GTESTCOVERAGE_CLANGCOVERAGEFILE_H
