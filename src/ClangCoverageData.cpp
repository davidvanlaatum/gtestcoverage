#include "ClangCoverageData.h"
#include "ClangCoverageFile.h"
#include "ClangCoverageFunction.h"
#include <stdserializers.h>

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
