#pragma once
#ifndef DVMON_GTESTCOVERAGELISTENER_H
#define DVMON_GTESTCOVERAGELISTENER_H

#include "gtestcoverage_export.h"
#include <gtest/gtest.h>
#include <memory>
#include "fwd.h"

namespace testing {
  namespace coverage {
    class GTESTCOVERAGE_EXPORT GTestCoverageListener : public ::testing::EmptyTestEventListener {
    public:
      GTestCoverageListener();
      ~GTestCoverageListener() override;
      void OnTestStart( const testing::TestInfo &info ) override;
      void OnTestEnd( const testing::TestInfo &info ) override;
      void OnTestProgramStart( const testing::UnitTest &test ) override;
      void OnTestProgramEnd( const testing::UnitTest &test ) override;

    protected:
      CoverageDataPtr data;
      std::vector<CoverageDriverPtr> drivers;
      CoverageDriverPtr driver;

      std::string fullTestCaseName( const testing::TestInfo &info ) const;
      std::string fullTestName( const testing::TestInfo &info ) const;
    };
  }
}

#endif //DVMON_GTESTCOVERAGELISTENER_H

