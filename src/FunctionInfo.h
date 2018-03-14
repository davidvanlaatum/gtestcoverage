#pragma once
#ifndef GTESTCOVERAGE_FUNCTIONINFO_H
#define GTESTCOVERAGE_FUNCTIONINFO_H

#include "fwd.h"
#include <memory>
#include <json.hpp>
#include <set>
#include <boost/filesystem/path.hpp>

namespace testing {
  namespace coverage {
    class FunctionInfo : public std::enable_shared_from_this<FunctionInfo> {
    public:
      FunctionInfo( const std::string &nName );
      void addSourceFile( const boost::filesystem::path &file );
      const std::string &getName() const;
      const std::set<boost::filesystem::path> &getSourceFiles() const;
      void addHits( uint32_t count );
      void addCoveringTest( const TestInfoPtr &test );
    protected:
      std::string name;
      std::set<boost::filesystem::path> sourceFiles;
      uint32_t hits{ 0 };
      std::map<std::string, TestInfoWeakPtr> tests;

      friend void from_json( const nlohmann::json &j, FunctionInfo &data );
      friend void to_json( nlohmann::json &j, const FunctionInfo &data );
    };

    void from_json( const nlohmann::json &j, FunctionInfo &data );
    void to_json( nlohmann::json &j, const FunctionInfo &data );
  }
}

#endif //GTESTCOVERAGE_FUNCTIONINFO_H
