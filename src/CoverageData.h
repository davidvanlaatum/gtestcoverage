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
      void loadFileList( const boost::filesystem::path &list );
      const FunctionInfoPtr &getFunction( const std::string &name );
      const FileInfoPtr &getFile( const boost::filesystem::path &name );
      void printSummary( std::ostream &os ) const;
      void setOutputFile( const boost::filesystem::path &name );
      const TestCaseInfoPtr &getTestCase( const std::string &name );
      void writeOutput() const;
      bool resolveSourceFile( const boost::filesystem::path &file, boost::filesystem::path &path ) const;
    protected:
      std::map<boost::filesystem::path, FileInfoPtr> files;
      std::map<std::string, FunctionInfoPtr> functions;
      boost::filesystem::path outputFile;
      boost::filesystem::path coversSourceDir;
      std::map<std::string, TestCaseInfoPtr> testCases;

      friend void from_json( const nlohmann::json &j, CoverageData &data );
      friend void to_json( nlohmann::json &j, const CoverageData &data );
    };

    void from_json( const nlohmann::json &j, CoverageData &data );
    void to_json( nlohmann::json &j, const CoverageData &data );
  }
}

#endif //GTESTCOVERAGE_COVERAGEDATA_H
