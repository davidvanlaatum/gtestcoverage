#include "LineInfo.h"
#include <FileInfo.h>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>
#include <utility>
#include <utility>

using namespace testing::coverage;

LineInfo::LineInfo( const FileInfoPtr &filePtr, size_t num ) : number( num ), file( filePtr ) {
  executedCount = 0;
}

void LineInfo::addExecutionCount( size_t count, const TestInfoPtr &test, const std::string &sourceCode ) {
  if ( test ) {
    executedCount += count;
    if ( count != 0u ) {
      tests.push_back( test );
      test->addLine( shared_from_this() );
    }
  }
  if ( code.empty() ) {
    code = sourceCode;
  }
}

bool LineInfo::isCovered() const {
  return executedCount > 0;
}

size_t LineInfo::getNumber() const {
  return number;
}

FileInfoPtr LineInfo::getFile() const {
  return file.lock();
}

void LineInfo::writeCobertura( tinyxml2::XMLElement *parent, tinyxml2::XMLDocument &doc ) const {
  tinyxml2::XMLElement *node = parent->InsertEndChild( doc.NewElement( "line" ) )->ToElement();
  node->SetAttribute( "branch", branches.empty() ? "false" : "true" );
  node->SetAttribute( "hits", static_cast<int>( executedCount) );
  node->SetAttribute( "number", static_cast<int>( number) );

  if ( !code.empty() ) {
    node->InsertEndChild( doc.NewText( code.c_str() ) );
    node->InsertEndChild( doc.NewText( "\n" ) );
  }

  for ( const auto &it : tests ) {
    tinyxml2::XMLElement *test = node->InsertEndChild( doc.NewElement( "test" ) )->ToElement();
    test->SetAttribute( "class", it->getSuite().c_str() );
    test->SetAttribute( "name", it->getName().c_str() );
  }
  std::string data = gcovData.str();
  if ( !data.empty() ) {
    node->InsertEndChild( doc.NewComment( data.c_str() ) );
  }

  if ( !branches.empty() ) {
    tinyxml2::XMLElement *conditions = node->InsertEndChild( doc.NewElement( "conditions" ) )->ToElement();
    std::map<size_t, size_t> branchCounts;
    std::map<size_t, size_t> branchHits;
    size_t hit = 0;
    for ( const auto &branche : branches ) {
      std::stringstream buffer;
      buffer << branche.second;
      conditions->InsertEndChild( doc.NewComment( buffer.str().c_str() ) );
      branchCounts[branche.second.block]++;
      if ( branche.second.hits != 0u ) {
        branchHits[branche.second.block]++;
        hit++;
      }
    }
    std::stringstream coverage;
    coverage << ( hit * 100 ) / branches.size() << "% (" << hit << "/" << branches.size() << ")";
    node->SetAttribute( "condition-coverage", coverage.str().c_str() );
    for ( auto &branchCount : branchCounts ) {
      tinyxml2::XMLElement *cond = conditions->InsertEndChild( doc.NewElement( "condition" ) )->ToElement();
      cond->SetAttribute( "number", static_cast<int>( branchCount.first) );
      cond->SetAttribute( "type", "jump" );
      std::stringstream buf;
      buf << ( branchHits[branchCount.first] * 100 ) / branchCount.second << "%";
      cond->SetAttribute( "coverage", buf.str().c_str() );
    }
  }
}

void LineInfo::addBranch( size_t num, size_t hits, size_t block, bool throws, const std::string &gcovDataString ) {
  Branch &branch = branches[num];
  branch.hits += hits;
  branch.block = block;
  branch.throws |= static_cast<int>(throws);
  if ( branch.gcovData.empty() || hits > 0 || throws ) {
    branch.gcovData = "block " + std::to_string( block ) + " " + gcovDataString;
  }
}

size_t LineInfo::getBranchCount() const {
  return branches.size();
}

size_t LineInfo::getBranchesCovered() const {
  size_t count = 0;

  for ( const auto &branch : branches ) {
    if ( branch.second.hits != 0u ) {
      count++;
    }
  }

  return count;
}

void LineInfo::cleanUp() {
  // Remove all branches that are for exceptions and the other half
  auto lastBranch = branches.end();
  for ( auto it = branches.begin(); it != branches.end(); ) {
    if ( it->second.throws ) {
      gcovData << ( gcovData.tellp() > 0 ? "\n" : "" );
      if ( lastBranch != branches.end() ) {
        gcovData << lastBranch->second << std::endl;
        branches.erase( lastBranch );
        lastBranch = branches.end();
      }
      gcovData << it->second;
      it = branches.erase( it );
    } else {
      lastBranch = it;
      ++it;
    }
  }
}

const LineInfo::branchesType &LineInfo::getBranches() const {
  return branches;
}

LineInfo::Branch::Branch() {
  hits = 0;
  block = 0;
  throws = false;
}

LineInfo::Branch::~Branch() {
  hits = 0;
  block = 0;
  throws = false;
  gcovData.clear();
}

std::ostream &::testing::coverage::operator<<( std::ostream &os, const testing::coverage::LineInfo::Branch &branch ) {
  os << branch.block << " " << branch.hits << " " << ( branch.throws ? "throws" : "nothrow" ) << " - "
     << branch.gcovData;
  return os;
}
