#pragma once
#ifndef GTESTCOVERAGE_CLANGCOVERAGEFWD_H
#define GTESTCOVERAGE_CLANGCOVERAGEFWD_H

#include <memory>
#include <json.hpp>
#include "fwd.h"

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

#endif //GTESTCOVERAGE_CLANGCOVERAGEFWD_H
