#include "FileInfo.h"
#include "TestInfo.h"
#include "TestCaseInfo.h"
#include "BlockInfo.h"
#include "LineInfo.h"
#include <stdserializers.h>

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
