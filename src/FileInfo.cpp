#include "FileInfo.h"
#include "BlockInfo.h"     // for BlockInfo
#include "TestCaseInfo.h"  // for TestCaseInfo
#include "TestInfo.h"      // for TestInfo
#include "LineInfo.h"      // IWYU pragma: keep
#include "CoverageData.h"
#include <utility>         // for pair
#include <stdserializers.h> // IWYU pragma: keep

using namespace testing::coverage;

FileInfo::FileInfo( const path &nName ) : name( nName ) {}

void testing::coverage::from_json( const nlohmann::json &j, testing::coverage::FileInfo &data ) {
  for ( const auto &item : j.value<std::set<std::string>>( "tests", {} ) ) {
    data.tests.emplace( item, TestInfoWeakPtr() );
  }
  for ( const auto &item : j.value<std::vector<BlockInfoPtr>>( "blocks", {} ) ) {
    data.blocks.emplace( item->getLocation(), item );
  }
  data.lines = j.value<decltype( data.lines )>( "lines", {} );
}

void testing::coverage::to_json( nlohmann::json &j, const testing::coverage::FileInfo &data ) {
  for ( const auto &item : data.tests ) {
    j["tests"].emplace_back( item.first );
  }
  for ( const auto &item : data.blocks ) {
    j["blocks"].emplace_back( item.second );
  }
  j["lines"] = data.lines;
}

void FileInfo::addCoveringTest( const TestInfoPtr &test ) {
  tests.emplace( test->getTestCase()->getName() + "::" + test->getName(), test );
}

const BlockInfoPtr &FileInfo::getBlock( const Block &block ) {
  auto &rt = blocks[block];
  if ( not rt ) {
    rt = std::make_shared<BlockInfo>( block );
  }
  return rt;
}

const path &FileInfo::getName() const {
  return name;
}

void FileInfo::setName( const path &nName ) {
  name = nName;
}

size_t FileInfo::getBlockCount() const {
  return blocks.size();
}

void FileInfo::readResolve( const CoverageDataPtr &data ) {
  for ( auto &item : tests ) {
    item.second = data->getTestByFullName( item.first );
  }
  for ( auto &item : lines ) {
    item.second->readResolve( data );
  }
  for ( auto &item : blocks ) {
    item.second->readResolve( data );
  }
}

bool FileInfo::hasCoverage() const {
  bool rt = false;
  for ( const auto &item : blocks ) {
    if ( item.second->getHits() ) {
      rt = true;
      break;
    }
  }
  return rt;
}

size_t FileInfo::getCoveredBlocks() const {
  size_t count = 0;
  for ( const auto &item : blocks ) {
    if ( item.second->getHits() ) {
      count++;
    }
  }
  return count;
}

size_t FileInfo::getLineCount() const {
  return lines.size();
}

size_t FileInfo::getCoveredLines() const {
  size_t count = 0;
  for ( const auto &item : lines ) {
    if ( item.second->hasCoverage() ) {
      count++;
    }
  }
  return count;
}

const LineInfoPtr &FileInfo::getLine( uint32_t line ) {
  auto &rt = lines[line];
  if ( not rt ) {
    rt = std::make_shared<LineInfo>( shared_from_this(), line );
  }
  return rt;
}

const std::map<LineNumber, LineInfoPtr> &FileInfo::getLines() const {
  return lines;
}
