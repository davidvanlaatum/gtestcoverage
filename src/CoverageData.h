#pragma once
#ifndef GTESTCOVERAGE_COVERAGEDATA_H
#define GTESTCOVERAGE_COVERAGEDATA_H

#include "fwd.h"
#include <memory>
#include <map>
#include <boost/filesystem/path.hpp>
#include <json.hpp>

namespace testing {
  namespace coverage {
    class CoverageData : public std::enable_shared_from_this<CoverageData> {
    public:
      CoverageData();
      void loadFileList( const path &list );
      const FunctionInfoPtr &getFunction( const std::string &name );
      const FileInfoPtr &getFile( const path &name );
      void printSummary( std::ostream &os ) const;
      void setOutputFile( const path &name );
      const TestCaseInfoPtr &getTestCase( const std::string &name );
      void writeOutput() const;
      bool resolveSourceFile( const path &file, path &path ) const;
    protected:
      std::map<path, FileInfoPtr> files;
      std::map<std::string, FunctionInfoPtr> functions;
      path outputFile;
      path coversSourceDir;
      std::map<std::string, TestCaseInfoPtr> testCases;

      friend void from_json( const nlohmann::json &j, CoverageData &data );
      friend void to_json( nlohmann::json &j, const CoverageData &data );
    };

    void from_json( const nlohmann::json &j, CoverageData &data );
    void to_json( nlohmann::json &j, const CoverageData &data );
  }
}

#endif //GTESTCOVERAGE_COVERAGEDATA_H
