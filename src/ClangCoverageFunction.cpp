#include "ClangCoverageFunction.h"
#include <CoverageData.h>           // for CoverageData
#include <FileInfo.h>               // for FileInfo
#include <boost/core/demangle.hpp>  // for demangle
#include <iostream>                 // for operator<<, basic_ostream, endl, basic_ostream<>::__ostream_type, clog
#include <utility>                  // for pair
#include <vector>                   // for vector
#include "BlockInfo.h"              // for BlockInfo
#include "FunctionInfo.h"           // for FunctionInfo
#include "MapDiff.h"                // for mapDiff
#include "Point.h"                  // for Point
#include "TestInfo.h"               // for TestInfo
#include <stdserializers.h> // IWYU pragma: keep

using namespace testing::coverage::clang;
using testing::coverage::path;

void testing::coverage::clang::from_json( const nlohmann::json &json, ClangCoverageFunction &data ) {
  data.name = json.at( "name" );
  data.hits = json.at( "count" );
  auto filenames = json["filenames"].get<std::vector<std::string>>();
  auto segments = json["regions"].get<std::vector<ClangCoverageFunctionSegmentPtr>>();
  data.segments.clear();
  for ( auto &segment : segments ) {
    segment->location.file = filenames[segment->fileId];
    data.segments.emplace( segment->getLocation(), segment );
  }
  for ( auto &file : filenames ) {
    data.sources.emplace( file );
  }
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, const ClangCoverageFunction &data ) {
  os << "Function: " << boost::core::demangle( data.name.c_str() ) << " hits: " << data.hits << std::endl;
  for ( const auto &segment : data.segments ) {
    std::clog << segment.second << std::endl;
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
  for ( const auto &segment : other.segments ) {
    segments[segment.first]->merge( *segment.second );
  }
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
  mapDiff( segments, other.segments, [&rt]( const ClangCoverageFunctionSegmentPtr &a, const ClangCoverageFunctionSegmentPtr &b ) -> bool {
      auto &seg = rt->segments[a ? a->getLocation() : b->getLocation()];
      if ( not seg ) {
        seg = std::make_shared<ClangCoverageFunctionSegment>( a ? a->getLocation() : b->getLocation() );
      }
      if ( a and b ) {
        seg->addHits( a->getHitCount() - b->getHitCount() );
      } else if ( a ) {
        seg->addHits( a->getHitCount() );
      }
      return false;
  } );
  return rt;
}

const std::set<path> &ClangCoverageFunction::getSources() const {
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

void ClangCoverageFunction::fill( const testing::coverage::TestInfoPtr &test, const testing::coverage::CoverageDataPtr &data ) const {
  auto demangledName = boost::core::demangle( name.c_str() );
  bool covered = false;
  if ( test->getCoveredFunctions().find( demangledName ) != test->getCoveredFunctions().end() ) {
    fill( data->getFunction( demangledName ) );
    covered = true;
  }
  for ( const auto &segment : segments ) {
    if ( covered or test->getCoveredFiles().find( segment.second->getFile() ) != test->getCoveredFiles().end() ) {
      auto file = data->getFile( segment.second->getFile() );
      auto block = file->getBlock( segment.second->getLocation() );
      block->addHits( segment.second->getHitCount() );
    }
  }
}

void testing::coverage::clang::from_json( const nlohmann::json &json, ClangCoverageFunctionSegment &data ) {
  data.location = BlockWithFilename( Point{ json[0], json[1] }, Point{ json[2], json[3] } );
  data.hitCount = json[4];
  data.fileId = json[5];
  data.expandedFileId = json[6];
  data.kind = json[7];
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, const ClangCoverageFunctionSegment &data ) {
  os << data.location.file.string() << ":" << data.location << " hits: " << data.hitCount << " kind: " << data.kind;
  return os;
}

ClangCoverageFunctionSegment::ClangCoverageFunctionSegment() = default;

ClangCoverageFunctionSegment::ClangCoverageFunctionSegment( const testing::coverage::BlockWithFilename &nLocation ) : location( nLocation ) {}

bool ClangCoverageFunctionSegment::operator==( const ClangCoverageFunctionSegment &rhs ) const {
  return location == rhs.location;
}

bool ClangCoverageFunctionSegment::operator!=( const ClangCoverageFunctionSegment &rhs ) const {
  return !( rhs == *this );
}

bool ClangCoverageFunctionSegment::contains( const ClangCoverageFunctionSegment &other ) const {
  return location.contains( other.location );
}

const testing::coverage::BlockWithFilename &ClangCoverageFunctionSegment::getLocation() const {
  return location;
}

uint32_t ClangCoverageFunctionSegment::getHitCount() const {
  return hitCount;
}

const path &ClangCoverageFunctionSegment::getFile() const {
  return location.file;
}

void ClangCoverageFunctionSegment::merge( const ClangCoverageFunctionSegment &other ) {
  location = other.location;
  hitCount += other.hitCount;
  kind = other.kind;
  fileId = other.fileId;
  expandedFileId = other.expandedFileId;
}

void ClangCoverageFunctionSegment::addHits( uint32_t count ) {
  hitCount += count;
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
