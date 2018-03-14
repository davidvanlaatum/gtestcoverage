#pragma once
#ifndef GTESTCOVERAGE_TESTCASEINFO_H
#define GTESTCOVERAGE_TESTCASEINFO_H

#include "fwd.h"
#include <memory>
#include <string>
#include <map>
#include <json.hpp>

namespace testing {
  namespace coverage {
    class TestCaseInfo : public std::enable_shared_from_this<TestCaseInfo> {
    public:
      TestCaseInfo( const std::string &nName );
      TestInfoPtr getTest( const std::string &testName );
      const std::string &getName() const;
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
