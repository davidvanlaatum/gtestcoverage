#pragma once
#ifndef GTESTCOVERAGE_FWD_H
#define GTESTCOVERAGE_FWD_H

#include <memory>
#include <iostream>

namespace testing {
  namespace coverage {
    class LineInfo;
    typedef std::shared_ptr<LineInfo> LineInfoPtr;
    class TestCaseInfo;
    typedef std::shared_ptr<TestCaseInfo> TestCaseInfoPtr;
    typedef std::weak_ptr<TestCaseInfo> TestCaseInfoWeakPtr;
    class TestInfo;
    typedef std::shared_ptr<TestInfo> TestInfoPtr;
    typedef std::weak_ptr<TestInfo> TestInfoWeakPtr;
    class FileInfo;
    typedef std::shared_ptr<FileInfo> FileInfoPtr;
    typedef std::weak_ptr<FileInfo> FileInfoWeakPtr;
    class CoverageDriver;
    typedef std::shared_ptr<CoverageDriver> CoverageDriverPtr;
    class CoverageData;
    typedef std::shared_ptr<CoverageData> CoverageDataPtr;
    typedef std::weak_ptr<CoverageData> CoverageDataWeakPtr;
    class FunctionInfo;
    typedef std::shared_ptr<FunctionInfo> FunctionInfoPtr;
  }
}

namespace std {
  template<typename T> inline auto operator<<( std::ostream &os, const std::shared_ptr<T> &ptr )
  -> decltype( os << std::declval<T>() ) {
    if ( ptr ) {
      os << *ptr;
    } else {
      os << "(null)";
    }
    return os;
  }
}

#endif //GTESTCOVERAGE_FWD_H
