#pragma once
#ifndef DVMON_COVERAGEDATA_H
#define DVMON_COVERAGEDATA_H

#include <map>
#include <vector>
#include <boost/filesystem.hpp>
#include "FileInfo.h"

namespace testing {
  namespace coverage {
    class CoverageData {
    public:
      typedef boost::filesystem::path path;
      void addFile( path file );
      void addCoversDir( const path &dir );
      void resolveFiles();
      void dumpStats();
      TestInfoPtr loadTestData( const std::string &suite, const std::string &name, bool passed );
      void addCovers( path file );
      void beginNewTest();
      void setOutputFile( const path &file );
    protected:
      typedef std::map<path, FileInfoPtr> filesType;
      filesType files;
      typedef std::vector<path> coveredDirsType;
      coveredDirsType coveredDirs;
      typedef std::vector<path> coverageFilesType;
      coverageFilesType coverageFiles;
      typedef std::vector<TestInfoPtr> testsType;
      testsType tests;
      typedef std::vector<path> coveredFilesType;
      coveredFilesType coveredFiles;
      path outputFile;

      void attachGCDAFile( const path &file );
      bool processGCovFile( const TestInfoPtr &test, const path &file );
      bool processGCDAFile( const TestInfoPtr &test, const path &file );
      bool processProfRaw( const TestInfoPtr &test, const path &file );
      bool isInteresting( const path &path );
      void writeOutputFile();
      void writeCobertura() const;
      void cleanUp();
    };
  }
}

#endif //DVMON_COVERAGEDATA_H
