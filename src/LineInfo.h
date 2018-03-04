#pragma once
#ifndef DVMON_LINEINFO_H
#define DVMON_LINEINFO_H

#include "gtestcoverage_export.h"
#include <string>
#include <list>
#include <map>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "TestInfo.h"
#include <boost/enable_shared_from_this.hpp>
#include <tinyxml2.h>

namespace testing {
  namespace coverage {
    class FileInfo;
    class GTESTCOVERAGE_EXPORT LineInfo : public boost::enable_shared_from_this<LineInfo> {
    public:
      struct Branch {
        Branch();
        Branch( const Branch &b ) = delete;
        ~Branch();
        size_t hits;
        size_t block;
        bool throws;
        std::string gcovData;
      };
      typedef std::map<size_t, Branch> branchesType;
      typedef boost::shared_ptr<LineInfo> Ptr;
      LineInfo( boost::weak_ptr<FileInfo> filePtr, size_t num );
      void addExecutionCount( size_t count, const TestInfo::Ptr &test, const std::string &sourceCode );
      bool isCovered() const;
      size_t getNumber() const;
      const boost::shared_ptr<FileInfo> getFile() const;
      void writeCobertura( tinyxml2::XMLElement *parent, tinyxml2::XMLDocument &doc ) const;
      void addBranch( size_t num, size_t hits, size_t block, bool throws, const std::string &gcovDataString );
      size_t getBranchCount() const;
      size_t getBranchesCovered() const;
      void cleanUp();
      const branchesType &getBranches() const;
    protected:
      branchesType branches;
      typedef std::list<TestInfo::Ptr> testsType;
      size_t number;
      testsType tests;
      size_t executedCount;
      boost::weak_ptr<FileInfo> file;
      std::stringstream gcovData;
      std::string code;
    };
    GTESTCOVERAGE_EXPORT std::ostream &operator<<( std::ostream &os, const LineInfo::Branch &branch );
  }
}

#endif //DVMON_LINEINFO_H
