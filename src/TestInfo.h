#pragma once
#ifndef DVMON_TESTINFO_H
#define DVMON_TESTINFO_H

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <map>
#include "fwd.h"

namespace testing {
  namespace coverage {
    class TestInfo : public std::enable_shared_from_this<TestInfo> {
    public:
      typedef std::vector<boost::filesystem::path> coveredFilesType;
      TestInfo( std::string nSuite, std::string nName, coveredFilesType nCoveredFiles,
                bool nPassed );
      const std::string &getName() const;
      const std::string &getSuite() const;
      void addLine( const LineInfoPtr &line );
    protected:
      std::string suite;
      std::string name;
      typedef std::map<boost::filesystem::path, size_t> coveredType;
      coveredType covered;
      coveredFilesType coveredFiles;
      bool passed;
      friend std::ostream &operator<<( std::ostream &os, const TestInfo &test );
    };

    std::ostream &operator<<( std::ostream &os, const TestInfo &test );
  }
}

#endif //DVMON_TESTINFO_H

