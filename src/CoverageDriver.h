#pragma once
#ifndef GTESTCOVERAGE_COVERAGEDRIVER_H
#define GTESTCOVERAGE_COVERAGEDRIVER_H

#include "fwd.h"
#include <string>
#include <memory>

namespace testing {
  namespace coverage {
    class CoverageDriver : public std::enable_shared_from_this<CoverageDriver> {
    public:
      virtual ~CoverageDriver() = default;
      virtual bool isAvailable( std::string &error ) const = 0;
      virtual const std::string getName() const = 0;
      virtual void startProgram() = 0;
      virtual void beginTest() = 0;
      virtual void endTest( const TestInfoPtr &test, const CoverageDataPtr &coverageData ) = 0;
      virtual void endProgram() = 0;
    };
  }
}

#endif //GTESTCOVERAGE_COVERAGEDRIVER_H
