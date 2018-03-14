#include "ClangCoverageDriver.h"
#include "ClangCoverageData.h"
#include <boost/filesystem.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/process.hpp>
#include <json.hpp>
#include <iostream>
#include "fwd.h"
#include "TestInfo.h"

using namespace boost::filesystem;
using namespace testing::coverage::clang;
using boost::process::child;

bool ClangCoverageDriver::isAvailable( std::string &error ) const {
  bool rt = false;
  if ( not getenv( "LLVM_PROFDATA" ) ) {
    error = "LLVM_PROFDATA env var is not set";
  } else if ( not getenv( "LLVM_SHOW" ) ) {
    error = "LLVM_SHOW env var is not set";
  } else if ( not getenv( "COVERS_FILE" ) ) {
    error = "COVERS_FILE env var is not set";
  } else {
    rt = true;
  }
  return rt;
}

const std::string ClangCoverageDriver::getName() const {
  return "clang";
}

void ClangCoverageDriver::startProgram() {
  directory_iterator dir( current_path() );
  while ( dir != directory_iterator() ) {
    if ( dir->path().extension() == ".profraw" ) {
      coverageFile = dir->path();
      tmpFile = coverageFile;
      tmpFile.replace_extension( ".profdata" );
      std::clog << "Found " << coverageFile.string() << std::endl;
      break;
    }
    dir++;
  }
}

void ClangCoverageDriver::beginTest() {
  if ( current ) {
    reference = std::move( current );
  } else {
    reference = loadData();
  }
  boost::system::error_code ec;
  remove( coverageFile, ec );
}

void ClangCoverageDriver::endTest( const TestInfoPtr &test, const CoverageDataPtr &coverageData ) {
  current = loadData();
  auto covered = current->diff( *reference );
//  std::clog << covered << std::endl;

  covered->fill( test, coverageData );
}

void ClangCoverageDriver::endProgram() {
  boost::system::error_code ec;
  remove( coverageFile, ec );
  remove( tmpFile, ec );
}

ClangCoverageDataPtr ClangCoverageDriver::loadData() const {
  auto exitcode = boost::process::system( getenv( "LLVM_PROFDATA" ), "merge", "-sparse", coverageFile, "-o", tmpFile );
  if ( exitcode != 0 ) {
    throw std::runtime_error( "LLVM_PROFDATA returned exit code " + std::to_string( exitcode ) );
  }
  auto data = std::make_shared<clang::ClangCoverageData>();
  try {
    boost::process::ipstream output;
    child exportjson( getenv( "LLVM_SHOW" ), "export", "-instr-profile", tmpFile, getenv( "COVERS_FILE" ), boost::process::std_out > output );
    nlohmann::json coverage;
    output >> coverage;
    data->merge( coverage );
    exportjson.wait();
    if ( exportjson.exit_code() != 0 ) {
      throw std::runtime_error( "LLVM_SHOW returned exit code " + std::to_string( exportjson.exit_code() ) );
    }
  } catch ( std::exception &e ) {
    std::clog << "Failed to run " << getenv( "LLVM_SHOW" ) << ": " << e.what() << std::endl;
  }
  try {
    boost::process::ipstream output;
    child exportjson( getenv( "LLVM_SHOW" ), "export", "-instr-profile", tmpFile, boost::dll::program_location(), boost::process::std_out > output );
    nlohmann::json coverage;
    output >> coverage;
    data->merge( coverage );
    exportjson.wait();
    if ( exportjson.exit_code() != 0 ) {
      throw std::runtime_error( "LLVM_SHOW returned exit code " + std::to_string( exportjson.exit_code() ) );
    }
  } catch ( std::exception &e ) {
    std::clog << "Failed to run " << getenv( "LLVM_SHOW" ) << ": " << e.what() << std::endl;
  }
  return data;
}
