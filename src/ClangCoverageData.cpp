#include "ClangCoverageData.h"
#include "ClangCoverageFile.h"              // for ClangCoverageFile
#include "ClangCoverageFunction.h"          // for ClangCoverageFunction, operator<<
#include "CoverageData.h"
#include "MapDiff.h"
#include "TestInfo.h"                       // for TestInfo
#include <boost/algorithm/string/join.hpp>  // for join
#include <boost/core/demangle.hpp>          // for demangle
#include <iostream>                         // for ostream, operator<<, char_traits, clog
#include <memory>                           // for __shared_ptr_access, shared_ptr, make_shared
#include <set>                              // for set, operator!=
#include <stdexcept>                        // for runtime_error
#include <stdserializers.h>                 // IWYU pragma: keep
#include <utility>                          // for pair, move

using namespace testing::coverage::clang;

void testing::coverage::clang::from_json( const nlohmann::json &json, ClangCoverageData &coverageData ) {
  if ( json.value<std::string>( "type", "" ) == "llvm.coverage.json.export" and json.value<std::string>( "version", "" ) == "2.0.0" ) {
    for ( const auto &data : json.at( "data" ) ) {
      for ( auto &file : data.value<std::vector<ClangCoverageFilePtr>>( "files", {} ) ) {
        coverageData.updateFile( std::move( file ) );
      }
      for ( auto &function : data.value<std::vector<ClangCoverageFunctionPtr>>( "functions", {} ) ) {
        coverageData.updateFunction( std::move( function ) );
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

  for ( const auto &file : data.files ) {
    const auto &existing = files.find( file.first );
    if ( existing == files.end() ) {
      files.emplace( file );
    } else {
      existing->second->merge( *file.second );
    }
  }
}

ClangCoverageDataPtr ClangCoverageData::diff( const ClangCoverageData &other ) const {
  auto rt = std::make_shared<ClangCoverageData>();

  mapDiff( functions, other.functions, [&rt]( const ClangCoverageFunctionPtr &a, const ClangCoverageFunctionPtr &b ) {
      ClangCoverageFunctionPtr func;
      if ( a and b ) {
        func = a->diff( *b );
      } else if ( a ) {
        func = std::make_shared<ClangCoverageFunction>( *a );
      } else {
        func = std::make_shared<ClangCoverageFunction>( *b );
        func->resetHits();
      }
      rt->functions.emplace( func->getName(), func );
      return false;
  } );

  mapDiff( files, other.files, [&rt]( const ClangCoverageFilePtr &a, const ClangCoverageFilePtr &b ) {
      ClangCoverageFilePtr file;
      if ( a and b ) {
        file = a->diff( *b );
      } else if ( a ) {
        file = std::make_shared<ClangCoverageFile>( *a );
      } else {
        file = std::make_shared<ClangCoverageFile>( *b );
        file->resetHits();
      }
      rt->files.emplace( file->getFilename(), file );
      return false;
  } );

  return rt;
}

ClangCoverageFunctionPtr ClangCoverageData::getFunction( const std::string &name ) const {
  auto item = functions.find( name );
  if ( item != functions.end() ) {
    return item->second;
  } else {
    return testing::coverage::clang::ClangCoverageFunctionPtr();
  }
}

ClangCoverageFilePtr ClangCoverageData::getFile( const testing::coverage::path &name ) const {
  auto item = files.find( name );
  if ( item != files.end() ) {
    return item->second;
  } else {
    return testing::coverage::clang::ClangCoverageFilePtr();
  }
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

  const auto &coveredFiles = test->getCoveredFiles();
  for ( const auto &file : files ) {
    file.second->fill( coveredFiles.find( file.second->getFilename() ) != coveredFiles.end() ? test : TestInfoPtr(), data );
  }
}
