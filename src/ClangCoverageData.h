#pragma once
#ifndef GTESTCOVERAGE_CLANGCOVERAGEDATA_H
#define GTESTCOVERAGE_CLANGCOVERAGEDATA_H

#include <json.hpp>
#include <boost/filesystem.hpp>
#include <map>
#include "ClangCoverageFwd.h"

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

      protected:
        std::map<path, ClangCoverageFilePtr> files;
        std::map<std::string, ClangCoverageFunctionPtr> functions;
        friend std::ostream &operator<<( std::ostream &os, const ClangCoverageData &data );
      };

      void from_json( const nlohmann::json &json, ClangCoverageData &data );
      std::ostream &operator<<( std::ostream &os, const ClangCoverageData &data );
    }
  }
}

#endif //GTESTCOVERAGE_CLANGCOVERAGEDATA_H
