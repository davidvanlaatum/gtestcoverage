#pragma once
#ifndef GTESTCOVERAGE_CLANGCOVERAGEDRIVER_H
#define GTESTCOVERAGE_CLANGCOVERAGEDRIVER_H

#include "CoverageDriver.h"
#include "ClangCoverageFwd.h"
#include <boost/filesystem/path.hpp>

namespace testing {
  namespace coverage {
    namespace clang {
      class ClangCoverageDriver : public CoverageDriver {
      public:
        bool isAvailable( std::string &error ) const override;
        const std::string getName() const override;
        void startProgram() override;
        void beginTest() override;
        void endTest( const TestInfoPtr &test, const CoverageDataPtr &coverageData ) override;
        void endProgram() override;

      protected:
        path coverageFile;
        path tmpFile;
        ClangCoverageDataPtr reference;
        ClangCoverageDataPtr current;

        ClangCoverageDataPtr loadData() const;
      };
    }
  }
}

#endif //GTESTCOVERAGE_CLANGCOVERAGEDRIVER_H
