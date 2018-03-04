#pragma once
#ifndef DVMON_FILEINFO_H
#define DVMON_FILEINFO_H

#include "gtestcoverage_export.h"
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "LineInfo.h"
#include <tinyxml2.h>

namespace testing {
  namespace coverage {
    class GTESTCOVERAGE_EXPORT FileInfo : public boost::enable_shared_from_this<FileInfo> {
    public:
      typedef boost::filesystem::path path;
      typedef boost::shared_ptr<FileInfo> Ptr;
      typedef std::map<size_t, LineInfo::Ptr> linesType;
      explicit FileInfo( path sourceFile );
      const path &getSource() const;
      size_t getTotalLines() const;
      size_t getCoveredLines() const;
      void processFile( const TestInfo::Ptr &test, std::istream &data );
      void setExplicitCovers();
      bool isExplicitCovered() const;
      void writeCobertura( tinyxml2::XMLElement *parent, tinyxml2::XMLDocument &doc ) const;
      void cleanUp();
      const linesType &getLines() const;
    protected:
      path source;
      linesType lines;
      bool explicitCovered;
      LineInfo::Ptr addLine( size_t lineNum, size_t executed, const TestInfo::Ptr &test, const std::string &code );
      void processLine( const TestInfo::Ptr &test, const std::string &line, LineInfo::Ptr &lastLine, size_t &blockNum );
    };
    std::ostream &operator<<( std::ostream &os, const FileInfo &file );
  }
}

#endif //DVMON_FILEINFO_H
