#include "GTestCoverageListener.h"
#include "ClangCoverageDriver.h"            // for ClangCoverageDriver
#include "CoverageData.h"                   // for CoverageData
#include "CoverageDriver.h"                 // for CoverageDriver
#include "FileInfo.h"                       // for FileInfo
#include "FunctionInfo.h"                   // for FunctionInfo
#include "GCCCoverageDriver.h"              // for GCCCoverageDriver
#include "GTestCoverageCovers.h"            // for COVERS_FILES_PROPERTY, COVERS_FUNCTIONS_PROPERTY, COVERS_NOTHING_PROPERTY
#include "NullCoverageDriver.h"             // for NullCoverageDriver
#include "TestCaseInfo.h"                   // for TestCaseInfo
#include "TestInfo.h"                       // for TestInfo
#include <gtest/gtest.h>                    // for TestInfo, TestProperty, TestResult, GTEST_LOG_, UnitTest (ptr only)
#include <boost/filesystem/operations.hpp>  // for exists
#include <cstdlib>                          // for getenv
#include <cstring>                          // for strcmp
#include <iostream>                         // for operator<<, basic_ostream, stringstream, endl, clog, ostream
#include <memory>                           // for allocator, __shared_ptr_access, make_shared, shared_ptr
#include <stdexcept>                        // for runtime_error

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
        path path;
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
