#include "CoverageData.h"
#include "FileInfo.h"                          // for FileInfo
#include "FunctionInfo.h"                      // for FunctionInfo
#include "TestCaseInfo.h"                      // for TestCaseInfo
#include "TestInfo.h"
#include <boost/filesystem/fstream.hpp>        // for ofstream, ifstream
#include <boost/filesystem/operations.hpp>     // for absolute, current_path, exists
#include <boost/filesystem/path.hpp>           // for path::reverse_iterator, operator<<, operator!=, path
#include <boost/iterator/iterator_facade.hpp>  // for operator!=, iterator_facade_base, iterator_facade
#include <cstdlib>                             // for getenv
#include <iostream>                            // for operator<<, basic_ostream, endl, ostream, clog, char_traits, basic_ostream<>::__ostream_type
#include <set>                                 // for set
#include <stdexcept>                           // for runtime_error
#include <type_traits>                         // for declval
#include <utility>                             // for pair
#include <stdserializers.h>                    // IWYU pragma: keep
#include <filesystemserializers.h>

using namespace testing::coverage;
using namespace boost::filesystem;

void testing::coverage::from_json( const nlohmann::json &j, CoverageData &data ) {
  if ( j.value<std::string>( "type", "" ) != "gtestcoverage" ) {
    throw std::runtime_error( "unsupported file format" );
  } else if ( j.value<std::string>( "version", "" ) != VERSION ) {
    throw std::runtime_error( "Version mismatch " VERSION );
  } else {
    data.coversSourceDir = j.value<boost::filesystem::path>( "coversDir", {} );
    data.testCases = j.value<decltype( data.testCases )>( "tests", {} );
    data.files = j.value<decltype( data.files )>( "files", {} );
    data.functions = j.value<decltype( data.functions )>( "functions", {} );
    data.readResolve();
  }
}

void testing::coverage::to_json( nlohmann::json &j, const CoverageData &data ) {
  j["version"] = VERSION;
  j["type"] = "gtestcoverage";
  j["coversDir"] = data.coversSourceDir;
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
  j["coverage"] = data.getCoverageStats();
}

const FunctionInfoPtr &CoverageData::getFunction( const std::string &name ) {
  if ( name.empty() ) {
    throw std::runtime_error( "Requested function with empty name" );
  }
  auto &rt = functions[name];
  if ( !rt ) {
    rt = std::make_shared<FunctionInfo>( name );
  }
  return rt;
}

const FileInfoPtr &CoverageData::getFile( const path &name ) {
  if ( name.empty() ) {
    throw std::runtime_error( "Requested file with empty name" );
  }
  auto &rt = files[name];
  if ( !rt ) {
    rt = std::make_shared<FileInfo>( name );
  }
  return rt;
}

void CoverageData::setOutputFile( const path &name ) {
  outputFile = name;
}

void CoverageData::loadFileList( const path &list ) {
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
  std::clog << "Loaded " << files.size() << " source files" << std::endl;
  if ( files.empty() ) {
    std::clog << "Failed to load any source files from " << list.native() << std::endl;
  }
}

void CoverageData::printSummary( std::ostream &os ) const {
  os << getCoverageStats() << std::endl;
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

bool endsWith( const path &heystack, const path &needle ) {
  bool rt = true;
  if ( needle.size() > heystack.size() ) {
    rt = false;
  } else {
    for ( auto it1 = needle.rbegin(), it2 = heystack.rbegin(); it1 != needle.rend() && it2 != heystack.rend(); ++it1, ++it2 ) {
      if ( *it1 != *it2 ) {
        rt = false;
        break;
      }
    }
  }
  return rt;
}

namespace std {
  template<typename T>
  auto operator<<( std::ostream &os, const std::set<T> &set )
  -> decltype( std::declval<std::ostream &>() << std::declval<T &>() ) {
    bool first = true;
    for ( const auto &item : set ) {
      if ( first ) {
        first = false;
      } else {
        os << ", ";
      }
      os << item;
    }
    return os;
  }
}

bool CoverageData::resolveSourceFile( const path &file, path &path ) const {
  bool rt = false;
  const auto &it = files.find( boost::filesystem::absolute( file, coversSourceDir ) );
  if ( it != files.end() ) {
    path = it->first;
    rt = true;
  }
  if ( not rt ) {
    std::set<coverage::path> options;
    for ( const auto &item : files ) {
      if ( endsWith( item.first, file ) ) {
        options.emplace( item.first );
      }
    }
    if ( options.size() == 1 ) {
      rt = true;
      path = *options.begin();
    } else if ( not options.empty() ) {
      std::clog << "Too many possible matches for " << file << ": " << options << std::endl;
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

void CoverageData::readResolve() {
  for ( const auto &item : functions ) {
    item.second->setName( item.first );
    item.second->readResolve( shared_from_this() );
  }
  for ( const auto &item : files ) {
    item.second->setName( item.first );
    item.second->readResolve( shared_from_this() );
  }
}

TestInfoPtr CoverageData::getTestByFullName( const std::string &name ) const {
  auto pos = name.find( "::" );
  auto testCase = testCases.find( name.substr( 0, pos ) );
  if ( testCase != testCases.end() ) {
    const auto &test = testCase->second->findTest( name.substr( pos + 2 ) );
    if ( test ) {
      return test;
    } else {
      throw std::runtime_error( "No such test " + name );
    }
  } else {
    throw std::runtime_error( "No such testcase " + name );
  }
}

CoverageStats CoverageData::getCoverageStats() const {
  CoverageStats stats;
  stats.files.count = files.size();
  for ( const auto &file : files ) {
    stats.blocks.count += file.second->getBlockCount();
    stats.blocks.covered += file.second->getCoveredBlocks();
    stats.lines.count += file.second->getLineCount();
    stats.lines.covered += file.second->getCoveredLines();
    if ( file.second->hasCoverage() ) {
      stats.files.covered++;
    }
  }
  stats.functions.count = functions.size();
  for ( const auto &func : functions ) {
    if ( func.second->hasCoverage() ) {
      stats.functions.covered++;
    }
  }
  return stats;
}

std::vector<FileInfoPtr> CoverageData::getFiles() const {
  std::vector<FileInfoPtr> rt;
  for ( const auto &item : files ) {
    rt.emplace_back( item.second );
  }
  return rt;
}

std::ostream &testing::coverage::operator<<( std::ostream &os, const CoverageStats &stats ) {
  os << "Files: " << stats.files << " Functions: " << stats.functions << " Blocks: " << stats.blocks << " Lines: " << stats.lines;
  return os;
}

std::ostream &testing::coverage::operator<<( std::ostream &os, const CoverageStats::Stats &stats ) {
  os << stats.covered << "/" << stats.count << "/" << std::setprecision( 3 ) << ( static_cast<double>(stats.covered) / stats.count ) * 100 << "%";
  return os;
}

void testing::coverage::to_json( nlohmann::json &j, const CoverageStats &stats ) {
  j["files"] = stats.files;
  j["functions"] = stats.functions;
  j["lines"] = stats.lines;
  j["blocks"] = stats.blocks;
}

void testing::coverage::to_json( nlohmann::json &j, const CoverageStats::Stats &stats ) {
  j["count"] = stats.count;
  j["covered"] = stats.covered;
}
