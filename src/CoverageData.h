#pragma once
#ifndef GTESTCOVERAGE_COVERAGEDATA_H
#define GTESTCOVERAGE_COVERAGEDATA_H

#include "fwd.h"     // for path, FileInfoPtr, FunctionInfoPtr, TestCaseInfoPtr
#include <iosfwd>    // for ostream
#include <json.hpp>  // for json
#include <map>       // for map
#include <memory>    // for enable_shared_from_this
#include <string>    // for string

namespace testing {
  namespace coverage {
    struct CoverageStats {
      struct Stats {
        size_t count{ 0 };
        size_t covered{ 0 };
      } functions, files, lines, blocks;
    };

    class CoverageData : public std::enable_shared_from_this<CoverageData> {
    public:
      CoverageData();
      void loadFileList( const path &list );
      const FunctionInfoPtr &getFunction( const std::string &name );
      FileInfoPtr getFile( const path &name ) const;
      void printSummary( std::ostream &os ) const;
      CoverageStats getCoverageStats() const;
      void setOutputFile( const path &name );
      const TestCaseInfoPtr &getTestCase( const std::string &name );
      TestInfoPtr getTestByFullName( const std::string &name ) const;
      void writeOutput() const;
      bool resolveSourceFile( const path &file, path &path ) const;
      void readResolve();
      std::vector<FileInfoPtr> getFiles() const;
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

    void to_json( nlohmann::json &j, const CoverageStats &stats );
    void to_json( nlohmann::json &j, const CoverageStats::Stats &stats );
    std::ostream &operator<<( std::ostream &os, const CoverageStats &stats );
    std::ostream &operator<<( std::ostream &os, const CoverageStats::Stats &stats );
  }
}

#endif //GTESTCOVERAGE_COVERAGEDATA_H
