#include "ClangCoverageFunction.h"
#include "FunctionInfo.h"
#include <boost/filesystem/path.hpp>
#include <boost/core/demangle.hpp>
#include <vector>
#include <iostream>
#include <boost/fusion/container/set.hpp>

using namespace testing::coverage::clang;

void testing::coverage::clang::from_json( const nlohmann::json &json, ClangCoverageFunction &data ) {
  data.name = json.at( "name" );
  data.hits = json.at( "count" );
  auto filenames = json["filenames"].get<std::vector<std::string>>();
  auto segments = json["regions"].get<std::vector<ClangCoverageFunctionSegment>>();
  data.segments.clear();
  for ( auto &segment : segments ) {
    if ( segment.kind == ClangCoverageFunctionSegment::CodeRegion ) {
      segment.file = filenames[segment.fileId];
      data.segments.emplace_back( segment );
    }
  }
  for ( auto &file : filenames ) {
    data.sources.emplace( file );
  }
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, const ClangCoverageFunction &data ) {
  os << "Function: " << boost::core::demangle( data.name.c_str() ) << " hits: " << data.hits << std::endl;
  for ( const auto &segment : data.segments ) {
    std::clog << segment << std::endl;
  }
  return os;
}

const std::string &ClangCoverageFunction::getName() const {
  return name;
}

void ClangCoverageFunction::merge( const ClangCoverageFunction &other ) {
  for ( const auto &file : other.sources ) {
    sources.emplace( file );
  }
  hits += other.hits;
}

ClangCoverageFunctionPtr ClangCoverageFunction::diff( const ClangCoverageFunction &other ) const {
  ClangCoverageFunctionPtr rt = std::make_shared<ClangCoverageFunction>();
  rt->name = name;
  rt->hits = hits - other.hits;
  for ( const auto &file : sources ) {
    rt->sources.emplace( file );
  }
  for ( const auto &file : other.sources ) {
    rt->sources.emplace( file );
  }
  return rt;
}

const std::set<boost::filesystem::path> &ClangCoverageFunction::getSources() const {
  return sources;
}

uint32_t ClangCoverageFunction::getHits() const {
  return hits;
}

void ClangCoverageFunction::fill( const testing::coverage::FunctionInfoPtr &function ) const {
  for ( const auto &file : sources ) {
    function->addSourceFile( file );
  }
  function->addHits( hits );
}

void testing::coverage::clang::from_json( const nlohmann::json &json, ClangCoverageFunctionSegment &data ) {
  data.start.line = json[0];
  data.start.column = json[1];
  data.end.line = json[2];
  data.end.column = json[3];
  data.hitCount = json[4];
  data.fileId = json[5];
  data.expandedFileId = json[6];
  data.kind = json[7];
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, const ClangCoverageFunctionSegment &data ) {
  os << data.file.string() << ":" << data.start << "-" << data.end << " hits: " << data.hitCount << " kind: " << data.kind;
  return os;
}

bool ClangCoverageFunctionSegment::operator==( const ClangCoverageFunctionSegment &rhs ) const {
  return start == rhs.start && end == rhs.end && file == rhs.file && kind == rhs.kind;
}

bool ClangCoverageFunctionSegment::operator!=( const ClangCoverageFunctionSegment &rhs ) const {
  return !( rhs == *this );
}

bool ClangCoverageFunctionSegment::contains( const ClangCoverageFunctionSegment &other ) const {
  bool rt = false;

  if ( start <= other.start && end >= other.end ) {
    rt = true;
  }

  return rt;
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, ClangCoverageFunctionSegment::RegionKind kind ) {
  switch ( kind ) {
    case ClangCoverageFunctionSegment::CodeRegion:
      os << "Code";
      break;
    case ClangCoverageFunctionSegment::ExpansionRegion:
      os << "Expansion";
      break;
    case ClangCoverageFunctionSegment::SkippedRegion:
      os << "Skipped";
      break;
    case ClangCoverageFunctionSegment::GapRegion:
      os << "Gap";
      break;
  }
  return os;
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, const ClangCoverageFunctionSegment::Point &point ) {
  os << point.line << ":" << point.column;
  return os;
}

bool ClangCoverageFunctionSegment::Point::operator==( const ClangCoverageFunctionSegment::Point &rhs ) const {
  return line == rhs.line && column == rhs.column;
}

bool ClangCoverageFunctionSegment::Point::operator!=( const ClangCoverageFunctionSegment::Point &rhs ) const {
  return !( rhs == *this );
}

bool ClangCoverageFunctionSegment::Point::operator<( const ClangCoverageFunctionSegment::Point &rhs ) const {
  if ( line < rhs.line ) {
    return true;
  }
  if ( rhs.line < line ) {
    return false;
  }
  return column < rhs.column;
}

bool ClangCoverageFunctionSegment::Point::operator>( const ClangCoverageFunctionSegment::Point &rhs ) const {
  return rhs < *this;
}

bool ClangCoverageFunctionSegment::Point::operator<=( const ClangCoverageFunctionSegment::Point &rhs ) const {
  return !( rhs < *this );
}

bool ClangCoverageFunctionSegment::Point::operator>=( const ClangCoverageFunctionSegment::Point &rhs ) const {
  return !( *this < rhs );
}
