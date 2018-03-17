#pragma once
#ifndef GTESTCOVERAGE_CLANGCOVERAGEFWD_H
#define GTESTCOVERAGE_CLANGCOVERAGEFWD_H

#include "fwd.h" // IWYU pragma: keep
#include <memory>

namespace testing {
  namespace coverage {
    namespace clang {
      class ClangCoverageData;
      typedef std::shared_ptr<ClangCoverageData> ClangCoverageDataPtr;
      class ClangCoverageFunction;
      typedef std::shared_ptr<ClangCoverageFunction> ClangCoverageFunctionPtr;
      class ClangCoverageFile;
      typedef std::shared_ptr<ClangCoverageFile> ClangCoverageFilePtr;
      class ClangCoverageFunctionSegment;
      typedef std::shared_ptr<ClangCoverageFunctionSegment> ClangCoverageFunctionSegmentPtr;
    }
  }
}

#endif //GTESTCOVERAGE_CLANGCOVERAGEFWD_H
