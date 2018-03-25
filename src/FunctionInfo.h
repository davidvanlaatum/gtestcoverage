#pragma once
#ifndef GTESTCOVERAGE_FUNCTIONINFO_H
#define GTESTCOVERAGE_FUNCTIONINFO_H

#include "fwd.h"     // for path, TestInfoWeakPtr, TestInfoPtr
#include <cstdint>   // for uint32_t
#include <json.hpp>  // for json
#include <map>       // for map
#include <memory>    // for enable_shared_from_this
#include <set>       // for set
#include <string>    // for string

namespace testing {
  namespace coverage {
    class FunctionInfo : public std::enable_shared_from_this<FunctionInfo> {
    public:
      FunctionInfo() = default;
      FunctionInfo( const std::string &nName );
      void addSourceFile( const path &file );
      const std::string &getName() const;
      void setName( const std::string &nName );
      const std::set<path> &getSourceFiles() const;
      void addHits( uint32_t count );
      void addCoveringTest( const TestInfoPtr &test );
      void readResolve( const CoverageDataPtr &data );
      bool hasCoverage() const;
    protected:
      std::string name;
      std::set<path> sourceFiles;
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
