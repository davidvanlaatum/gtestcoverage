#include "GTestCoverageListener.h"
#include "CoverageData.h"
#include <cstddef>
#include <fstream>
#include <gtest/gtest.h>

extern "C" {
void main_coverage_reset();
void main_coverage_dump();
void unit_coverage_reset();
void unit_coverage_dump();
}

using namespace testing::coverage;
using namespace boost::filesystem;
using namespace std;

GTestCoverageListener::GTestCoverageListener() {
  data = std::make_shared<CoverageData>();
}

GTestCoverageListener::~GTestCoverageListener() = default;

void GTestCoverageListener::OnTestStart( const testing::TestInfo & ) {
  main_coverage_reset();
  unit_coverage_reset();
  data->beginNewTest();
}

void GTestCoverageListener::OnTestEnd( const testing::TestInfo &info ) {
  const TestResult *pResult = info.result();
  bool found = false;
  for ( int i = 0; i < pResult->test_property_count(); i++ ) {
    const TestProperty &aProperty = pResult->GetTestProperty( i );
    if ( strcmp( aProperty.key(), "COVERS" ) == 0 ) {
      std::stringstream x( aProperty.value() );
      std::string file;
      while ( !x.eof() ) {
        std::getline( x, file, ':' );
        data->addCovers( file );
      }
      found = true;
    } else if ( strcmp( aProperty.key(), "COVERS_NOTHING" ) == 0 ) {
      found = true;
    }
  }
  if ( !found ) {
    GTEST_LOG_( WARNING ) << info.test_case_name() << "::" << info.name() << " does not specify what it covers";
  }
  main_coverage_dump();
  unit_coverage_dump();
  data->loadTestData( info.test_case_name(), info.name(), info.result()->Passed() );
}

void GTestCoverageListener::OnTestProgramStart( const testing::UnitTest &test __attribute__((unused)) ) {
  main_coverage_reset();
  unit_coverage_reset();
  main_coverage_dump();
  unit_coverage_dump();
  loadSources();
}

void GTestCoverageListener::OnTestProgramEnd( const testing::UnitTest &test __attribute__((unused)) ) {
  data->dumpStats();
}

void GTestCoverageListener::loadSources() {
  if ( std::getenv( "OBJECTS_DIRS" ) != nullptr ) {
    string cdirs = std::getenv( "OBJECTS_DIRS" );
    size_t pos = cdirs.find( ':' );
    size_t lpos = 0;
    while ( pos != string::npos ) {
      data->addCoversDir( cdirs.substr( lpos, pos - lpos ) );
      lpos = pos + 1;
      pos = cdirs.find( ':', lpos );
    }
    data->addCoversDir( cdirs.substr( lpos ) );
  }
  if ( getenv( "SOURCE_LIST" ) != nullptr ) {
    if ( exists( getenv( "SOURCE_LIST" ) ) ) {
      std::ifstream sources( getenv( "SOURCE_LIST" ), std::ios_base::in );
      while ( !sources.eof() ) {
        std::string file;
        std::getline( sources, file );
        if ( !file.empty() ) {
          data->addFile( file );
        }
      }
    }
  }
  if ( getenv( "COVERAGE_FILE" ) != nullptr ) {
    data->setOutputFile( getenv( "COVERAGE_FILE" ) );
  }
  data->resolveFiles();
}
