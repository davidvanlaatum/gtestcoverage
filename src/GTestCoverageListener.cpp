#include "GTestCoverageListener.h"
#include "CoverageData.h"
#include "FileInfo.h"
#include "ClangCoverageDriver.h"
#include "GCCCoverageDriver.h"
#include "NullCoverageDriver.h"
#include <cstddef>
#include <cstring>
#include <fstream>
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include "TestCaseInfo.h"
#include "TestInfo.h"
#include "GTestCoverageCovers.h"
#include "FunctionInfo.h"

extern "C" {
void main_coverage_reset();
void main_coverage_dump();
const char *main_driver_name() __attribute__((pure));
void unit_coverage_reset();
void unit_coverage_dump();
const char *unit_driver_name() __attribute__((pure));
}

using namespace testing::coverage;
using namespace boost::filesystem;
using namespace std;

GTestCoverageListener::GTestCoverageListener() {
  data = std::make_shared<CoverageData>();
  drivers.push_back( std::make_shared<clang::ClangCoverageDriver>() );
  drivers.push_back( std::make_shared<gcc::GCCCoverageDriver>() );
  drivers.push_back( std::make_shared<NullCoverageDriver>() );
}

GTestCoverageListener::~GTestCoverageListener() = default;

void GTestCoverageListener::OnTestStart( const testing::TestInfo & ) {
  main_coverage_reset();
  unit_coverage_reset();
  driver->beginTest();
}

void GTestCoverageListener::OnTestEnd( const testing::TestInfo &info ) {
  const TestResult *pResult = info.result();
  auto testCase = data->getTestCase( info.test_case_name() );
  auto test = testCase->getTest( fullTestName( info ) );
  test->setSuccess( pResult->Passed() );
  bool found = false;
  for ( int i = 0; i < pResult->test_property_count(); i++ ) {
    const TestProperty &aProperty = pResult->GetTestProperty( i );
    if ( strcmp( aProperty.key(), COVERS_FILES_PROPERTY ) == 0 ) {
      std::stringstream x( aProperty.value() );
      std::string file;
      while ( !x.eof() ) {
        std::getline( x, file, ':' );
        boost::filesystem::path path;
        if ( data->resolveSourceFile( file, path ) ) {
          test->addCoveredFile( path );
          data->getFile( path )->addCoveringTest( test );
        } else {
          GTEST_LOG_( WARNING ) << fullTestCaseName( info ) << " failed to resolve file: '" << file << "'";
        }
      }
      found = true;
    } else if ( strcmp( aProperty.key(), COVERS_NOTHING_PROPERTY ) == 0 ) {
      found = true;
    } else if ( strcmp( aProperty.key(), COVERS_FUNCTIONS_PROPERTY ) == 0 ) {
      found = true;
      std::stringstream x( aProperty.value() );
      std::string function;
      while ( !x.eof() ) {
        std::getline( x, function );
        test->addCoveredFunction( function );
        data->getFunction( function )->addCoveringTest( test );
      }
    }
  }
  if ( !found ) {
    GTEST_LOG_( WARNING ) << fullTestCaseName( info ) << " does not specify what it covers";
  }
  main_coverage_dump();
  unit_coverage_dump();
  driver->endTest( test, data );
}

void GTestCoverageListener::OnTestProgramStart( const testing::UnitTest & ) {
  main_coverage_reset();
  unit_coverage_reset();
  main_coverage_dump();
  unit_coverage_dump();
  auto sourcesPath = std::getenv( "SOURCE_LIST" );
  if ( sourcesPath && exists( sourcesPath ) ) {
    data->loadFileList( sourcesPath );
  } else if ( sourcesPath ) {
    std::clog << "Missing sources file: " << sourcesPath << std::endl;
  } else {
    std::clog << "No source list" << std::endl;
  }
  if ( std::getenv( "COVERAGE_FILE" ) != nullptr ) {
    data->setOutputFile( std::getenv( "COVERAGE_FILE" ) );
  } else {
    std::clog << "No output file coverage will not be saved!" << std::endl;
  }
  std::string driverName = main_driver_name();
  if ( driverName != unit_driver_name() ) {
    throw std::runtime_error( "unit vs main driver mismatch: " + driverName + "!=" + unit_driver_name() );
  }
  for ( const auto &d : drivers ) {
    if ( d->getName() == driverName ) {
      std::string error;
      if ( !d->isAvailable( error ) ) {
        throw std::runtime_error( "Required driver is not available: " + error );
      } else {
        driver = d;
      }
      break;
    }
  }
  if ( !driver ) {
    throw std::runtime_error( "Failed to find driver " + driverName );
  }
  driver->startProgram();
}

void GTestCoverageListener::OnTestProgramEnd( const testing::UnitTest & ) {
  driver->endProgram();
  data->printSummary( std::clog );
  data->writeOutput();
}

std::string GTestCoverageListener::fullTestCaseName( const testing::TestInfo &info ) const {
  std::stringstream out;
  out << info.test_case_name() << "::" << fullTestName( info );
  return out.str();
}

std::string GTestCoverageListener::fullTestName( const testing::TestInfo &info ) const {
  std::stringstream out;
  out << info.name();

  if ( info.type_param() ) {
    out << "::" << info.type_param();
  }
  if ( info.value_param() ) {
    out << "::" << info.value_param();
  }

  return out.str();
}
