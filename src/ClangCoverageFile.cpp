#include "ClangCoverageFile.h"
#include <ostream>  // for operator<<, basic_ostream
#include <string>   // for string

using namespace testing::coverage::clang;
using testing::coverage::path;

void testing::coverage::clang::from_json( const nlohmann::json &json, ClangCoverageFileSegment &data ) {
  data.line = json[0];
  data.column = json[1];
  data.hits = json[2];
  data.hasCount = json[3] > 0;
  data.isLine = json[4] == 0;
}

std::ostream &testing::coverage::clang::operator<<( std::ostream &os, const ClangCoverageFileSegment &data ) {
  os << "Line: " << data.line;
  if ( not data.isLine ) {
    os << " Column: " << data.column;
  }
  os << " Hits: " << data.hits;
  os << " has: " << data.hasCount;
  return os;
}

void testing::coverage::clang::from_json( const nlohmann::json &json, ClangCoverageFile &data ) {
  data.filename = json["filename"].get<std::string>();
//  std::clog << "File " << data.filename << std::endl;
//  std::vector<ClangCoverageFileSegment> segments;
//  segments = json["segments"].get<std::vector<ClangCoverageFileSegment>>();
//  for ( const auto &segment : segments ) {
//    std::clog << segment << std::endl;
//  }
}

const path &ClangCoverageFile::getFilename() const {
  return filename;
}

void ClangCoverageFile::merge( const ClangCoverageFile & ) {
}
