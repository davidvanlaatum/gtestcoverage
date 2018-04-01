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
  auto filenames = json.value<std::vector<std::string>>( "filenames", {} );
  data.segments.clear();
  uint32_t index = 0;
  for ( auto &segment : json.value<std::vector<ClangCoverageFunctionSegmentPtr>>( "regions", {} ) ) {
    segment->location.file = filenames[segment->location.fileId];
    segment->fileIndex = index++;
    data.segments.emplace( segment->getLocation(), segment );
  }
  for ( auto &file : filenames ) {
    data.sources.emplace( file );
  }
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, const ClangCoverageFunction &data ) {
  os << "Function: " << boost::core::demangle( data.name.c_str() ) << " hits: " << data.hits << std::endl;
  auto segments = data.getSegmentsByFileOrder();
  for ( const auto &segment : segments ) {
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
      if ( a and b ) {
        seg = a->diff( *b );
      } else if ( a ) {
        seg = std::make_shared<ClangCoverageFunctionSegment>( *a );
      } else {
        seg = std::make_shared<ClangCoverageFunctionSegment>( *b );
        seg->resetHits();
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
  bool coveredByFile = false;
  if ( test->getCoveredFunctions().find( demangledName ) != test->getCoveredFunctions().end() ) {
    covered = true;
  }
  for ( const auto &segment : segments ) {
    if ( covered or test->getCoveredFiles().find( segment.second->getFile() ) != test->getCoveredFiles().end() ) {
      coveredByFile = true;
      auto file = data->getFile( segment.second->getFile() );
      auto block = file->getBlock( segment.second->getLocation() );
      block->addHits( segment.second->getHitCount() );
      block->addCoveringTest( test );
    }
  }
  if ( covered or coveredByFile ) {
    fill( data->getFunction( demangledName ) );
  }
}

void ClangCoverageFunction::resetHits() {
  hits = 0;
  for ( const auto &item : segments ) {
    item.second->resetHits();
  }
}

std::vector<ClangCoverageFunctionSegmentPtr> ClangCoverageFunction::getSegmentsByFileOrder() const {
  std::vector<ClangCoverageFunctionSegmentPtr> rt;
  std::for_each( segments.begin(), segments.end(), [&rt]( decltype( *segments.begin() ) &value ) { rt.push_back( value.second ); } );
  std::sort( rt.begin(), rt.end(), []( ClangCoverageFunctionSegmentPtr &a, ClangCoverageFunctionSegmentPtr &b ) {
      return a->getFileIndex() < b->getFileIndex();
  } );
  return rt;
}

void testing::coverage::clang::from_json( const nlohmann::json &json, ClangCoverageFunctionSegment &data ) {
  data.location = ClangCoverageFunctionSegment::locationType( Point{ json[0], json[1] }, Point{ json[2], json[3] } );
  data.hitCount = json[4];
  data.location.fileId = json[5];
  data.location.expandedFileId = json[6];
  data.location.kind = json[7];
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, const ClangCoverageFunctionSegment &data ) {
  os << data.location << " Index: " << data.fileIndex << " hits: " << data.hitCount << " kind: " << data.location.kind;
  return os;
}

ClangCoverageFunctionSegment::ClangCoverageFunctionSegment() = default;

ClangCoverageFunctionSegment::ClangCoverageFunctionSegment( const locationType &nLocation ) : location( nLocation ) {}

bool ClangCoverageFunctionSegment::operator==( const ClangCoverageFunctionSegment &rhs ) const {
  return location == rhs.location;
}

bool ClangCoverageFunctionSegment::operator!=( const ClangCoverageFunctionSegment &rhs ) const {
  return !( rhs == *this );
}

bool ClangCoverageFunctionSegment::contains( const ClangCoverageFunctionSegment &other ) const {
  return location.contains( other.location );
}

const ClangCoverageFunctionSegment::locationType &ClangCoverageFunctionSegment::getLocation() const {
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
}

void ClangCoverageFunctionSegment::addHits( uint32_t count ) {
  hitCount += count;
}

void ClangCoverageFunctionSegment::resetHits() {
  hitCount = 0;
}

ClangCoverageFunctionSegmentPtr ClangCoverageFunctionSegment::diff( const ClangCoverageFunctionSegment &other ) const {
  ClangCoverageFunctionSegmentPtr rt = std::make_shared<ClangCoverageFunctionSegment>( location );
  rt->hitCount = hitCount - other.hitCount;
  rt->fileIndex = fileIndex;
  return rt;
}

uint32_t ClangCoverageFunctionSegment::getFileIndex() const {
  return fileIndex;
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, ClangCoverageFunctionSegment::RegionKind kind ) {
  switch ( kind ) {
    case ClangCoverageFunctionSegment::Unknown:
      os << "Unknown";
      break;
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

ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId::ClangCoverageFunctionSegmentId() : BlockWithFilename() {
}

ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId::ClangCoverageFunctionSegmentId( const testing::coverage::Point &nStart,
                                                                                              const testing::coverage::Point &nEnd ) : BlockWithFilename(
    nStart, nEnd ) {
}

bool ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId::operator==( const ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId &rhs ) const {
  return static_cast<const testing::coverage::BlockWithFilename &>(*this) == static_cast<const testing::coverage::BlockWithFilename &>(rhs) &&
         kind == rhs.kind &&
         fileId == rhs.fileId &&
         expandedFileId == rhs.expandedFileId;
}

bool ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId::operator!=( const ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId &rhs ) const {
  return !( rhs == *this );
}

bool ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId::operator<( const ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId &rhs ) const {
  if ( static_cast<const testing::coverage::BlockWithFilename &>(*this) < static_cast<const testing::coverage::BlockWithFilename &>(rhs) ) {
    return true;
  }
  if ( static_cast<const testing::coverage::BlockWithFilename &>(rhs) < static_cast<const testing::coverage::BlockWithFilename &>(*this) ) {
    return false;
  }
  if ( kind < rhs.kind ) {
    return true;
  }
  if ( rhs.kind < kind ) {
    return false;
  }
  if ( fileId < rhs.fileId ) {
    return true;
  }
  if ( rhs.fileId < fileId ) {
    return false;
  }
  return expandedFileId < rhs.expandedFileId;
}

bool ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId::operator>( const ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId &rhs ) const {
  return rhs < *this;
}

bool ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId::operator<=( const ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId &rhs ) const {
  return !( rhs < *this );
}

bool ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId::operator>=( const ClangCoverageFunctionSegment::ClangCoverageFunctionSegmentId &rhs ) const {
  return !( *this < rhs );
}
