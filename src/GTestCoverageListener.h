#pragma once
#ifndef DVMON_GTESTCOVERAGELISTENER_H
#define DVMON_GTESTCOVERAGELISTENER_H

#include "gtestcoverage_export.h"
#include <gtest/gtest.h>
#include <boost/smart_ptr/shared_ptr.hpp>

namespace testing {
  namespace coverage {
    class CoverageData;
    typedef boost::shared_ptr<CoverageData> CoverageDataPtr;
    class GTESTCOVERAGE_EXPORT GTestCoverageListener : public ::testing::EmptyTestEventListener {
    public:
      GTestCoverageListener();
      ~GTestCoverageListener() override;
      void OnTestStart( const TestInfo &info ) override;
      void OnTestEnd( const TestInfo &info ) override;
      void OnTestProgramStart( const UnitTest &test ) override;
      void OnTestProgramEnd( const UnitTest &test ) override;

    protected:
      CoverageDataPtr data;

      void loadSources();
    };
  }
}

#endif //DVMON_GTESTCOVERAGELISTENER_H

