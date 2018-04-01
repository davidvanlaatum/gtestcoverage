#include "SourceFile.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

using namespace testing::coverage;

SourceFile::SourceFile( const path &filename ) {
  if ( boost::filesystem::exists( filename ) ) {
    auto size = boost::filesystem::file_size( filename );
    boost::filesystem::ifstream sourceFile( filename );
    contents.resize( size );
    sourceFile.read( contents.data(), static_cast<std::streamsize>(size) );

    uint32_t line = 1;
    index[line] = 0;
    size_t i = 0;
    for ( const auto &c : contents ) {
      if ( c == '\n' ) {
        index[++line] = i + 1;
      }
      i++;
    }
    for ( auto &c : index ) {
      if ( contents[c.second] == '\r' ) {
        c.second++;
      }
    }
  }
}

bool SourceFile::lineHasCode( uint32_t line, Point start, Point end ) const {
  bool rt = false;
  const auto x = index.find( line );
  if ( x != index.end() ) {
    std::string_view section = contents.data();
    if ( end.line == start.line ) {
      section = section.substr( x->second + start.column - 1, end.column - start.column );
    } else {
      const auto y = index.find( line + 1 );
      if ( y != index.end() ) {
        section = section.substr( x->second + start.column - 1, y->second - x->second - start.column );
      } else {
        section = section.substr( x->second + start.column - 1 );
      }
    }
    if ( not section.empty() ) {
      if ( section.find_first_not_of( " \n\t{};" ) != std::string::npos ) {
        rt = true;
      }
    }
  }
  return rt;
}
