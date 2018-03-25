#pragma once
#ifndef GTESTCOVERAGE_CLANGCOVERAGEDATA_H
#define GTESTCOVERAGE_CLANGCOVERAGEDATA_H

#include "fwd.h"               // for path, CoverageDataPtr, TestInfoPtr
#include "ClangCoverageFwd.h"  // for ClangCoverageFilePtr, ClangCoverageFunctionPtr, ClangCoverageDataPtr
#include <iosfwd>              // for ostream
#include <json.hpp>            // for json
#include <map>                 // for map
#include <string>              // for string

namespace testing {
  namespace coverage {
    namespace clang {
      class ClangCoverageData {
      public:
        void updateFile( ClangCoverageFilePtr &&file );
        void updateFunction( ClangCoverageFunctionPtr &&function );
        void merge( const ClangCoverageData &data );
        ClangCoverageDataPtr diff( const ClangCoverageData &other ) const;
        void fill( const TestInfoPtr &test, const CoverageDataPtr &data ) const;
        ClangCoverageFunctionPtr getFunction( const std::string &name ) const;
        ClangCoverageFilePtr getFile( const path &name ) const;

      protected:
        std::map<path, ClangCoverageFilePtr> files;
        std::map<std::string, ClangCoverageFunctionPtr> functions;
        friend std::ostream &operator<<( std::ostream &os, const ClangCoverageData &data );
      };

      void from_json( const nlohmann::json &json, ClangCoverageData &coverageData );
      std::ostream &operator<<( std::ostream &os, const ClangCoverageData &data );
    }
  }
}

#endif //GTESTCOVERAGE_CLANGCOVERAGEDATA_H
