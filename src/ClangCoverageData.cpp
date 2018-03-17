#include "ClangCoverageData.h"
#include "ClangCoverageFile.h"              // for ClangCoverageFile
#include "ClangCoverageFunction.h"          // for ClangCoverageFunction, operator<<
#include "TestInfo.h"                       // for TestInfo
#include <boost/algorithm/string/join.hpp>  // for join
#include <boost/core/demangle.hpp>          // for demangle
#include <iostream>                         // for ostream, operator<<, char_traits, clog
#include <memory>                           // for __shared_ptr_access, shared_ptr, make_shared
#include <set>                              // for set, operator!=
#include <stdexcept>                        // for runtime_error
#include <utility>                          // for pair, move
#include <stdserializers.h> // IWYU pragma: keep

using namespace testing::coverage::clang;

void testing::coverage::clang::from_json( const nlohmann::json &json, ClangCoverageData &coverageData ) {
  if ( json.at( "type" ) == "llvm.coverage.json.export" && json.at( "version" ) == "2.0.0" ) {
    for ( const auto &data : json["data"] ) {
      for ( const auto &file : data["files"] ) {
        ClangCoverageFilePtr f;
        f = file;
        coverageData.updateFile( std::move( f ) );
      }
      for ( const auto &function : data["functions"] ) {
        ClangCoverageFunctionPtr func;
        func = function;
        coverageData.updateFunction( std::move( func ) );
      }
    }
  }
}

void ClangCoverageData::updateFile( ClangCoverageFilePtr &&file ) {
  const auto &existing = files.find( file->getFilename() );
  if ( existing != files.end() ) {
    existing->second->merge( *file );
  } else {
    files[file->getFilename()] = std::move( file );
  }
}

void ClangCoverageData::updateFunction( ClangCoverageFunctionPtr &&function ) {
  const auto &existing = functions.find( function->getName() );
  if ( existing != functions.end() ) {
    existing->second->merge( *function );
  } else {
    functions[function->getName()] = std::move( function );
  }
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, const ClangCoverageData &data ) {
  for ( const auto &function : data.functions ) {
    os << function.second;
  }
  return os;
}

void ClangCoverageData::merge( const ClangCoverageData &data ) {
  for ( const auto &function : data.functions ) {
    const auto &existing = functions.find( function.first );
    if ( existing == functions.end() ) {
      functions.emplace( function );
    } else {
      existing->second->merge( *function.second );
    }
  }
}

ClangCoverageDataPtr ClangCoverageData::diff( const ClangCoverageData &other ) const {
  auto rt = std::make_shared<ClangCoverageData>();
  std::set<std::string> left, right, all;

  for ( const auto &function : functions ) {
    left.emplace( function.first );
  }
  for ( const auto &function : other.functions ) {
    right.emplace( function.first );
  }

  if ( left != right ) {
    std::clog << "Function name mismatch";
    all.insert( left.begin(), left.end() );
    all.insert( right.begin(), right.end() );
  } else {
    all = left;
  }

  for ( const auto &func : all ) {
    const auto &leftFunc = functions.find( func );
    const auto &rightFunc = other.functions.find( func );
    ClangCoverageFunctionPtr result;
    if ( leftFunc != functions.end() && rightFunc != other.functions.end() ) {
      result = leftFunc->second->diff( *rightFunc->second );
    } else if ( leftFunc != functions.end() ) {
      result = std::make_shared<ClangCoverageFunction>( *leftFunc->second );
    } else {
      result = std::make_shared<ClangCoverageFunction>( *rightFunc->second );
    }
    rt->functions.emplace( result->getName(), result );
  }

  return rt;
}

void ClangCoverageData::fill( const testing::coverage::TestInfoPtr &test, const testing::coverage::CoverageDataPtr &data ) const {
  std::set<std::string> demangled;
  const auto &coveredFunctions = test->getCoveredFunctions();
  for ( const auto &func : functions ) {
    auto demangledName = boost::core::demangle( func.first.c_str() );
    demangled.emplace( demangledName );
    func.second->fill( test, data );
  }

  for ( const auto &func : coveredFunctions ) {
    if ( demangled.find( func ) == demangled.end() ) {
      throw std::runtime_error( "Function " + func + " not found in coverage data\n" + boost::join( demangled, "\n" ) );
    }
  }
}
