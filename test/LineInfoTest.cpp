#include <FileInfo.h>
//#include <IsIn.h>
//#include <StreamUtils.h>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing::coverage;
using testing::IsEmpty;
using testing::SizeIs;
#if 0

TEST( LineInfoTest, Test1 ) {
  RecordProperty( "COVERS_NOTHING", 1 );
  FileInfoPtr info = std::make_shared<FileInfo>( "abc123" );
  std::ifstream file1( TEST_LOCATION "/test1.1.gcov", std::ios_base::in );
  info->processFile( TestInfoPtr(), file1 );
  std::ifstream file2( TEST_LOCATION "/test1.2.gcov", std::ios_base::in );
  info->processFile( TestInfoPtr(), file2 );
  info->cleanUp();
  auto it = info->getLines().find( 38 );
  ASSERT_NE( it, info->getLines().end() );
  EXPECT_THAT( it->second->getBranches(), IsEmpty() );
}

TEST( LineInfoTest, Test2 ) {
  RecordProperty( "COVERS_NOTHING", 1 );
  FileInfoPtr info = std::make_shared<FileInfo>( "abc123" );
  for ( auto name : {
      TEST_LOCATION "/test2.1.gcov",
      TEST_LOCATION "/test2.2.gcov",
      TEST_LOCATION "/test2.3.gcov"
  } ) {
    std::ifstream file( name, std::ios_base::in );
    info->processFile( TestInfoPtr(), file );
  }
  info->cleanUp();
  auto it = info->getLines().find( 14 );
  ASSERT_NE( it, info->getLines().end() );
  auto &branches = it->second->getBranches();
  EXPECT_THAT( branches, SizeIs( 2 ) );
  EXPECT_EQ( branches.at( 2 ).hits, 1 );
  EXPECT_EQ( branches.at( 3 ).hits, 1 );
}

#endif
