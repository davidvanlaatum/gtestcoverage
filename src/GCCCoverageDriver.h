#pragma once
#ifndef GTESTCOVERAGE_GCCCOVERAGEDRIVER_H
#define GTESTCOVERAGE_GCCCOVERAGEDRIVER_H

#include "CoverageDriver.h"

namespace testing {
  namespace coverage {
    namespace gcc {
      class GCCCoverageDriver : public CoverageDriver {
      public:
        bool isAvailable( std::string &error ) const override;
        const std::string getName() const override;
        void startProgram() override;
        void beginTest() override;
        void endTest( const TestInfoPtr &test, const CoverageDataPtr &coverageData ) override;
        void endProgram() override;
      };
    }
  }
}

#endif //GTESTCOVERAGE_GCCCOVERAGEDRIVER_H
