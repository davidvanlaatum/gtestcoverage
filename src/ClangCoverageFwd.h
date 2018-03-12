#pragma once
#ifndef GTESTCOVERAGE_CLANGCOVERAGEFWD_H
#define GTESTCOVERAGE_CLANGCOVERAGEFWD_H

#include <memory>
#include <json.hpp>
#include <iosfwd>

namespace testing {
  namespace coverage {
    namespace clang {
      class ClangCoverageData;
      typedef std::shared_ptr<ClangCoverageData> ClangCoverageDataPtr;
      class ClangCoverageFunction;
      typedef std::shared_ptr<ClangCoverageFunction> ClangCoverageFunctionPtr;
      class ClangCoverageFile;
      typedef std::shared_ptr<ClangCoverageFile> ClangCoverageFilePtr;
    }
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

#endif //GTESTCOVERAGE_CLANGCOVERAGEFWD_H
