#pragma once
#ifndef GTESTCOVERAGE_TESTCASEINFO_H
#define GTESTCOVERAGE_TESTCASEINFO_H

#include "fwd.h"     // for TestInfoPtr
#include <json.hpp>  // for json
#include <map>       // for map
#include <memory>    // for enable_shared_from_this
#include <string>    // for string

namespace testing {
  namespace coverage {
    class TestCaseInfo : public std::enable_shared_from_this<TestCaseInfo> {
    public:
      TestCaseInfo() = default;
      TestCaseInfo( const std::string &nName );
      TestInfoPtr getTest( const std::string &testName );
      TestInfoPtr findTest( const std::string &testName );
      const std::string &getName() const;
      void setName( const std::string &nName );
    protected:
      std::string name;
      std::map<std::string, TestInfoPtr> tests;

      friend void from_json( const nlohmann::json &j, TestCaseInfo &data );
      friend void to_json( nlohmann::json &j, const TestCaseInfo &data );
    };
    void from_json( const nlohmann::json &j, TestCaseInfo &data );
    void to_json( nlohmann::json &j, const TestCaseInfo &data );
  }
}

#endif //GTESTCOVERAGE_TESTCASEINFO_H
