#pragma once
#ifndef DVMON_FILEINFO_H
#define DVMON_FILEINFO_H

#include "gtestcoverage_export.h"
#include <map>
#include <boost/filesystem.hpp>
#include "LineInfo.h"
#include <tinyxml2.h>

namespace testing {
  namespace coverage {
    class GTESTCOVERAGE_EXPORT FileInfo : public std::enable_shared_from_this<FileInfo> {
    public:
      typedef boost::filesystem::path path;
      typedef std::map<size_t, LineInfoPtr> linesType;
      explicit FileInfo( path sourceFile );
      const path &getSource() const;
      size_t getTotalLines() const;
      size_t getCoveredLines() const;
      void processFile( const TestInfoPtr &test, std::istream &data );
      void setExplicitCovers();
      bool isExplicitCovered() const;
      void writeCobertura( tinyxml2::XMLElement *parent, tinyxml2::XMLDocument &doc ) const;
      void cleanUp();
      const linesType &getLines() const;
    protected:
      path source;
      linesType lines;
      bool explicitCovered;
      LineInfoPtr addLine( size_t lineNum, size_t executed, const TestInfoPtr &test, const std::string &code );
      void processLine( const TestInfoPtr &test, const std::string &line, LineInfoPtr &lastLine, size_t &blockNum );
    };
    std::ostream &operator<<( std::ostream &os, const FileInfo &file );
  }
}

#endif //DVMON_FILEINFO_H
