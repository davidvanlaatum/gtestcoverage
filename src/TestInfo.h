#pragma once
#ifndef GTESTCOVERAGE_TESTINFO_H
#define GTESTCOVERAGE_TESTINFO_H

#include "fwd.h"
#include <memory>
#include <json.hpp>
#include <string>
#include <set>
#include <boost/filesystem/path.hpp>

namespace testing {
  namespace coverage {
    class TestInfo : public std::enable_shared_from_this<TestInfo> {
    public:
      TestInfo( const std::string &nName, const TestCaseInfoPtr &pTestCase );
      const std::string &getName() const;
      TestCaseInfoPtr getTestCase() const;
      bool isSuccess() const;
      void setSuccess( bool nSuccess );
      void addCoveredFile( const path &file );
      void addCoveredFunction( const std::string &functionName );
      const std::set<path> &getCoveredFiles() const;
      const std::set<std::string> &getCoveredFunctions() const;
    protected:
      std::string name;
      TestCaseInfoWeakPtr testCase;
      bool success{ false };
      std::set<path> coveredFiles;
      std::set<std::string> coveredFunctions;

      friend void from_json( const nlohmann::json &j, TestInfo &data );
      friend void to_json( nlohmann::json &j, const TestInfo &data );
    };

    void from_json( const nlohmann::json &j, TestInfo &data );
    void to_json( nlohmann::json &j, const TestInfo &data );
  }
}

#endif //GTESTCOVERAGE_TESTINFO_H
