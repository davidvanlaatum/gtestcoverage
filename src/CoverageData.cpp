#include "CoverageData.h"
#include "FunctionInfo.h"
#include "FileInfo.h"
#include <iostream>
#include <stdserializers.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include <set>
#include "TestCaseInfo.h"

using namespace testing::coverage;
using namespace boost::filesystem;

//void testing::coverage::from_json( const nlohmann::json &j, CoverageData &data ) {
//}

void testing::coverage::to_json( nlohmann::json &j, const CoverageData &data ) {
  j["version"] = VERSION;
  j["type"] = "gtestcoverage";
  auto &jTests = j["tests"];
  for ( const auto &testCase : data.testCases ) {
    jTests.emplace( testCase.first, testCase.second );
  }
  auto &jFunctions = j["functions"];
  for ( const auto &function : data.functions ) {
    jFunctions.emplace( function.first, function.second );
  }
  auto &jFiles = j["files"];
  for ( const auto &file : data.files ) {
    jFiles.emplace( file.first.string(), file.second );
  }
}

const FunctionInfoPtr &CoverageData::getFunction( const std::string &name ) {
  auto &rt = functions[name];
  if ( !rt ) {
    rt = std::make_shared<FunctionInfo>( name );
  }
  return rt;
}

const FileInfoPtr &CoverageData::getFile( const boost::filesystem::path &name ) {
  auto &rt = files[name];
  if ( !rt ) {
    rt = std::make_shared<FileInfo>( name );
  }
  return rt;
}

void CoverageData::setOutputFile( const boost::filesystem::path &name ) {
  outputFile = name;
}

void CoverageData::loadFileList( const boost::filesystem::path &list ) {
  ifstream sources{ list };
  while ( !sources.eof() ) {
    std::string file;
    std::getline( sources, file );
    if ( !file.empty() ) {
      if ( exists( file ) ) {
        files.emplace( file, std::make_shared<FileInfo>( file ) );
      } else {
        std::clog << "Missing source file " << file << std::endl;
      }
    }
  }
  if ( files.empty() ) {
    std::clog << "Failed to load any source files from " << list.native() << std::endl;
  }
}

void CoverageData::printSummary( std::ostream &os ) const {
  os << "Files: " << files.size() << " Functions: " << functions.size() << std::endl;
}

const TestCaseInfoPtr &CoverageData::getTestCase( const std::string &name ) {
  auto &rt = testCases[name];
  if ( not rt ) {
    rt = std::make_shared<TestCaseInfo>( name );
  }
  return rt;
}

void CoverageData::writeOutput() const {
  if ( not outputFile.empty() ) {
    ofstream outFile;
    outFile.open( outputFile );
    nlohmann::json j;
    j = *this;
    outFile << j;
    outFile.close();
  } else {
    nlohmann::json j;
    j = *this;
    std::clog << j.dump( 2 ) << std::endl;
  }
}

bool endsWith( const boost::filesystem::path &heystack, const boost::filesystem::path &needle ) {
  bool rt = true;
  for ( auto it1 = needle.rbegin(), it2 = heystack.rbegin(); it1 != needle.rend() && it2 != heystack.rend(); ++it1, ++it2 ) {
    if ( *it1 != *it2 ) {
      rt = false;
      break;
    }
  }
  return rt;
}

bool CoverageData::resolveSourceFile( const boost::filesystem::path &file, boost::filesystem::path &path ) const {
  bool rt = false;
  const auto &it = files.find( boost::filesystem::absolute( file, coversSourceDir ) );
  if ( it != files.end() ) {
    path = it->first;
    rt = true;
  }
  if ( not rt ) {
    std::set<boost::filesystem::path> options;
    for ( const auto &item : files ) {
      if ( endsWith( item.first, file ) ) {
        options.emplace( item.first );
      }
    }
    if ( options.size() == 1 ) {
      rt = true;
      path = *options.begin();
    }
  }
  return rt;
}

CoverageData::CoverageData() {
  if ( std::getenv( "COVERS_SOURCE_DIR" ) ) {
    coversSourceDir = std::getenv( "COVERS_SOURCE_DIR" );
  } else {
    coversSourceDir = boost::filesystem::current_path();
  }
}
