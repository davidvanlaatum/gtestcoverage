#pragma once
#ifndef GTESTCOVERAGE_TESTINFO_H
#define GTESTCOVERAGE_TESTINFO_H

#include "fwd.h"     // for path, TestCaseInfoPtr, TestCaseInfoWeakPtr
#include <json.hpp>  // for json
#include <memory>    // for enable_shared_from_this
#include <set>       // for set
#include <string>    // for string

namespace testing {
  namespace coverage {
    class TestInfo : public std::enable_shared_from_this<TestInfo> {
    public:
      TestInfo() = default;
      TestInfo( const std::string &nName, const TestCaseInfoPtr &pTestCase );
      TestInfo( std::string &&nName, const TestCaseInfoPtr &pTestCase );
      const std::string &getName() const;
      std::string getFullName() const;
      void setName( const std::string &nName );
      TestCaseInfoPtr getTestCase() const;
      void setTestCase( const TestCaseInfoPtr &pTestCase );
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
