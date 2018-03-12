#pragma once
#ifndef GTESTCOVERAGE_FWD_H
#define GTESTCOVERAGE_FWD_H

#include <memory>

namespace testing {
  namespace coverage {
    class LineInfo;
    typedef std::shared_ptr<LineInfo> LineInfoPtr;
    class TestInfo;
    typedef std::shared_ptr<TestInfo> TestInfoPtr;
    class FileInfo;
    typedef std::shared_ptr<FileInfo> FileInfoPtr;
    typedef std::weak_ptr<FileInfo> FileInfoWeakPtr;
  }
}

#endif //GTESTCOVERAGE_FWD_H
