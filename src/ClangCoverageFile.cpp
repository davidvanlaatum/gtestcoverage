#include "ClangCoverageFile.h"
#include <ostream>  // for operator<<, basic_ostream
#include <string>   // for string
#include <deque>
#include <boost/filesystem/fstream.hpp>
#include "MapDiff.h"
#include "TestInfo.h"
#include "CoverageData.h"
#include "FileInfo.h"
#include "LineInfo.h"
#include "SourceFile.h"
#include "CLangCoverageException.h"

using namespace testing::coverage::clang;
using testing::coverage::path;

void testing::coverage::clang::from_json( const nlohmann::json &json, ClangCoverageFileSegment &data ) {
  data.address.line = json[0];
  data.address.column = json[1];
  data.hits = json[2];
  data.hasCount = json[3] > 0;
  data.address.newSegment = json[4] > 0;
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, const ClangCoverageFileSegment &data ) {
  os << "Line: " << data.address.line;
  os << " Column: " << data.address.column;
  os << " Hits: " << data.hits;
  os << " has: " << data.hasCount;
  os << " new Segment " << data.address.newSegment;
  return os;
}

const ClangCoverageFileSegment::LineAddress &ClangCoverageFileSegment::getAddress() const {
  return address;
}

bool ClangCoverageFileSegment::isNewSegment() const {
  return address.newSegment;
}

uint32_t ClangCoverageFileSegment::getLine() const {
  return address.line;
}

void ClangCoverageFileSegment::merge( const ClangCoverageFileSegment &other ) {
  hits += other.hits;
}

ClangCoverageFileSegmentPtr ClangCoverageFileSegment::diff( const ClangCoverageFileSegment &other ) const {
  auto rt = std::make_shared<ClangCoverageFileSegment>( *this );
  rt->hits = hits - other.hits;
  return rt;
}

void ClangCoverageFileSegment::resetHits() {
  hits = 0;
}

uint32_t ClangCoverageFileSegment::getHits() const {
  return hits;
}

bool ClangCoverageFileSegment::isHasCount() const {
  return hasCount;
}

void testing::coverage::clang::from_json( const nlohmann::json &json, ClangCoverageFile &data ) {
  data.filename = json.value<path>( "filename", {} );
  auto segments = json.value<std::vector<ClangCoverageFileSegmentPtr>>( "segments", {} );
  for ( const auto &segment : segments ) {
    data.segments.emplace( segment->getAddress(), segment );
  }
  data.resolveLines();
}

const path &ClangCoverageFile::getFilename() const {
  return filename;
}

void ClangCoverageFile::merge( const ClangCoverageFile &other ) {
  mapDiff( segments, other.segments, [&]( const ClangCoverageFileSegmentPtr &a, const ClangCoverageFileSegmentPtr &b ) -> bool {
      if ( a and b ) {
        a->merge( *b );
      } else if ( b ) {
        auto segment = std::make_shared<ClangCoverageFileSegment>( *b );
        segments.emplace( segment->getAddress(), segment );
      }
      return false;
  } );
}

void ClangCoverageFile::resolveLines() {
  SourceFile source( filename );
  std::deque<ClangCoverageFileSegmentPtr> stack;

  auto it = segments.begin();
  auto next = it;
  Point currentPos = static_cast<Point>(it->second->getAddress());
  ClangLineInfoPtr lastLine;
  while ( it != segments.end() ) {
    ++next;
    Point end = next == segments.end() ? Point() : static_cast<Point>(next->second->getAddress());
    if ( it->second->isNewSegment() ) {
      stack.emplace_back( it->second );
    }
    if ( not it->second->isHasCount() ) {
      currentPos = end;
    } else {
      while ( currentPos < end ) {
        if ( not lastLine or lastLine->getLine() != currentPos.line ) {
          if ( not stack.empty() ) {
            if ( source.lineHasCode( currentPos.line, currentPos, end ) ) {
              lastLine = getLine( currentPos.line );
              lastLine->addHits( stack.back()->getHits() );
            }
          } else {
            currentPos = end;
            break;
          }
        }
        if ( currentPos.line == end.line ) {
          currentPos = end;
        } else {
          currentPos.line++;
          currentPos.column = 1;
        }
      }
      if ( not it->second->isNewSegment() ) {
        if ( stack.empty() ) {
          throw CLangCoverageException( "Unbalanced segments for file " + filename.native() );
        }
        stack.pop_back();
      }
    }
    ++it;
  }
}

ClangCoverageFilePtr ClangCoverageFile::diff( const ClangCoverageFile &other ) const {
  auto rt = std::make_shared<ClangCoverageFile>();
  rt->filename = filename;

  mapDiff( segments, other.segments, [&rt]( const ClangCoverageFileSegmentPtr &a, const ClangCoverageFileSegmentPtr &b ) {
      ClangCoverageFileSegmentPtr segment;
      if ( a and b ) {
        segment = a->diff( *b );
      } else if ( a ) {
        segment = std::make_shared<ClangCoverageFileSegment>( *a );
      } else if ( b ) {
        segment = std::make_shared<ClangCoverageFileSegment>( *b );
        segment->resetHits();
      }
      rt->segments.emplace( segment->getAddress(), std::move( segment ) );
      return false;
  } );

  rt->resolveLines();

  return rt;
}

void ClangCoverageFile::resetHits() {
  for ( const auto &item : segments ) {
    item.second->resetHits();
  }
  for ( const auto &item : lines ) {
    item.second->resetHits();
  }
}

void ClangCoverageFile::fill( const testing::coverage::TestInfoPtr &test, const testing::coverage::CoverageDataPtr &data ) const {
  auto file = data->getFile( filename );
  if ( file ) {
    for ( const auto &item : lines ) {
      const auto &line = file->getLine( item.first );
      if ( item.second->getHits() ) {
        line->addHits( item.second->getHits() );
        line->addCoveringTest( test );
      }
    }
  }
}

bool ClangCoverageFileSegment::LineAddress::operator<( const ClangCoverageFileSegment::LineAddress &rhs ) const {
  if ( line < rhs.line ) {
    return true;
  }
  if ( rhs.line < line ) {
    return false;
  }
  if ( column < rhs.column ) {
    return true;
  }
  if ( rhs.column < column ) {
    return false;
  }
  return newSegment < rhs.newSegment;
}

bool ClangCoverageFileSegment::LineAddress::operator>( const ClangCoverageFileSegment::LineAddress &rhs ) const {
  return rhs < *this;
}

bool ClangCoverageFileSegment::LineAddress::operator<=( const ClangCoverageFileSegment::LineAddress &rhs ) const {
  return !( rhs < *this );
}

bool ClangCoverageFileSegment::LineAddress::operator>=( const ClangCoverageFileSegment::LineAddress &rhs ) const {
  return !( *this < rhs );
}

bool ClangCoverageFileSegment::LineAddress::operator==( const ClangCoverageFileSegment::LineAddress &rhs ) const {
  return line == rhs.line &&
         column == rhs.column &&
         newSegment == rhs.newSegment;
}

bool ClangCoverageFileSegment::LineAddress::operator!=( const ClangCoverageFileSegment::LineAddress &rhs ) const {
  return !( rhs == *this );
}

uint32_t ClangLineInfo::getHits() const {
  return hits;
}

void ClangLineInfo::addHits( uint32_t count ) {
  hits += count;
}

void ClangLineInfo::resetHits() {
  hits = 0;
}

uint32_t ClangLineInfo::getLine() const {
  return line;
}
