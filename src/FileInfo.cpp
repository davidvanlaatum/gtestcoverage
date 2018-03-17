#include "FileInfo.h"
#include "BlockInfo.h"     // for BlockInfo
#include "TestCaseInfo.h"  // for TestCaseInfo
#include "TestInfo.h"      // for TestInfo
#include "LineInfo.h"      // IWYU pragma: keep
#include <utility>         // for pair
#include <stdserializers.h> // IWYU pragma: keep

using namespace testing::coverage;

FileInfo::FileInfo( const path &nName ) : name( nName ) {}

//void testing::coverage::from_json( const nlohmann::json &j, testing::coverage::FileInfo &data ) {
//}

void testing::coverage::to_json( nlohmann::json &j, const testing::coverage::FileInfo &data ) {
  for ( const auto &item : data.tests ) {
    j["tests"].emplace_back( item.first );
  }
  for ( const auto &item : data.blocks ) {
    j["blocks"].emplace_back( item.second );
  }
  for ( const auto &item : data.lines ) {
    j["lines"].emplace( std::to_string( item.first ), item.second );
  }
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
