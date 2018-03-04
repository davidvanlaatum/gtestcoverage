#include "FileInfo.h"
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <utility>

using namespace testing::coverage;
using namespace std;

ostream &testing::coverage::operator<<( ostream &os, const FileInfo &file ) {
  os << file.getSource() << ": " << file.getCoveredLines() << " of " << file.getTotalLines() << " lines covered";
  return os;
}

FileInfo::FileInfo( FileInfo::path sourceFile ) : source( std::move( sourceFile ) ) {
  explicitCovered = false;
}

const FileInfo::path &FileInfo::getSource() const {
  return source;
}

size_t FileInfo::getTotalLines() const {
  return lines.size();
}

size_t FileInfo::getCoveredLines() const {
  size_t count = 0;

  for ( const auto &line : lines ) {
    if ( line.second->isCovered() ) {
      count++;
    }
  }

  return count;
}

bool startsWith( const std::string &s1, const std::string &s2 ) {
  bool rt = false;

  if ( s1.size() >= s2.size() ) {
    rt = true;
    for ( std::string::const_iterator i1 = s1.begin(), i2 = s2.begin(); i1 != s1.end() && i2 != s2.end(); ++i1, ++i2 ) {
      if ( *i1 != *i2 ) {
        rt = false;
        break;
      }
    }
  }

  return rt;
}

void FileInfo::processFile( const TestInfo::Ptr &test, std::istream &data ) {
#ifdef LOG_GCOV_OUTPUT
  std::string name = source.string();
  if ( test ) {
    name += "_" + test->getSuite() + "_" + test->getName();
  }
  size_t p;
  while ( ( p = name.find( '/' ) ) != std::string::npos ) {
    name.replace( p, 1, "_" );
  }
  std::ofstream log( name, std::ios_base::out );
#endif
  std::string line;
  LineInfo::Ptr lastLine;
  size_t blockNum = 0;
  bool inIgnore = false;
  while ( data ) {
    std::getline( data, line );
#ifdef LOG_GCOV_OUTPUT
    log << line << std::endl;
#endif
    if ( line.find( "GCOV_EXCL_LINE" ) != std::string::npos ) {
      lastLine.reset();
    } else if ( line.find( "GCOV_EXCL_START" ) != std::string::npos ) {
      lastLine.reset();
      inIgnore = true;
    } else if ( inIgnore ) {
      if ( line.find( "GCOV_EXCL_END" ) != std::string::npos ) {
        inIgnore = false;
      }
    } else if ( startsWith( line, "function" ) ) {
    } else if ( startsWith( line, "branch" ) ) {
      if ( lastLine ) {
        std::string::const_iterator it = line.begin();
        it += static_cast<int>( strlen( "branch" ));
        std::string num;
        std::string hits = "0";
        while ( it != line.end() && *it == ' ' ) { ++it; }
        while ( it != line.end() && *it >= '0' && *it <= '9' ) {
          num += *it;
          ++it;
        }
        while ( it != line.end() && *it == ' ' ) { ++it; }
        if ( it != line.end() && *it == 't' ) { ++it; }
        if ( it != line.end() && *it == 'a' ) { ++it; }
        if ( it != line.end() && *it == 'k' ) { ++it; }
        if ( it != line.end() && *it == 'e' ) { ++it; }
        if ( it != line.end() && *it == 'n' ) { ++it; }
        while ( it != line.end() && *it == ' ' ) { ++it; }
        if ( *it >= '0' && *it <= '9' ) {
          while ( it != line.end() && *it >= '0' && *it <= '9' ) {
            hits += *it;
            ++it;
          }
        }

        bool throws = line.find( "(throw)" ) != std::string::npos;

        try {
          lastLine->addBranch( boost::lexical_cast<size_t>( num ), boost::lexical_cast<size_t>( hits ), blockNum,
                               throws,
                               line );
        } catch ( boost::bad_lexical_cast & ) {
          GTEST_LOG_( WARNING ) << "Failed to parse branch '" << num << "' '" << hits << "'";
        }
      }
    } else if ( startsWith( line, "call" ) ) {
    } else {
      processLine( test, line, lastLine, blockNum );
    }
  }
}

void FileInfo::processLine( const TestInfo::Ptr &test, const string &line, LineInfo::Ptr &lastLine, size_t &blockNum ) {
  std::string count;
  std::string lineNum;

  std::string::const_iterator it = line.begin();
  while ( it != line.end() ) {
    if ( *it == ':' ) {
      ++it;
      break;
    } else if ( *it != ' ' ) {
      count += *it;
    }
    ++it;
  }

  while ( it != line.end() ) {
    if ( *it == '-' ) {
      break;
    } else if ( *it == ':' ) {
      break;
    } else if ( *it != ' ' ) {
      lineNum += *it;
    }
    ++it;
  }

  if ( !count.empty() && !lineNum.empty() && count != "-" ) {
    if ( count[0] == '#' || count[0] == '=' || count[0] == '$' || count[0] == '%' ) {
      count = "0";
    }
    if ( *it == '-' ) {
      ++it;
      std::string block;
      block.append( it, line.end() );
      try {
        blockNum = boost::lexical_cast<size_t>( block.substr( block.find_last_not_of( "0123456789" ) + 1 ) );
      } catch ( boost::bad_lexical_cast & ) {
        GTEST_LOG_( WARNING ) << "Failed to parse block number " << block;
      }
    } else {
      try {
        std::string code( ++it, line.end() );
        lastLine = addLine( boost::lexical_cast<size_t>( lineNum ), boost::lexical_cast<size_t>( count ), test, code );
        blockNum = 0;
      } catch ( boost::bad_lexical_cast & ) {
        GTEST_LOG_( WARNING ) << "Failed to parse line '" << lineNum << "' '" << count << "'";
      }
    }
  }
}

LineInfo::Ptr FileInfo::addLine( size_t lineNum, size_t executed, const TestInfo::Ptr &test, const string &code ) {
  LineInfo::Ptr line;
  auto it = lines.find( lineNum );
  if ( it != lines.end() ) {
    line = it->second;
  } else {
    line = boost::make_shared<LineInfo>( shared_from_this(), lineNum );
    lines[lineNum] = line;
  }
  line->addExecutionCount( executed, test, code );
  return line;
}

void FileInfo::setExplicitCovers() {
  explicitCovered = true;
}

bool FileInfo::isExplicitCovered() const {
  return explicitCovered;
}

boost::filesystem::path trim( const boost::filesystem::path &path ) {
  boost::filesystem::path rt = path;

  if ( std::getenv( "SOURCE_DIR" ) != nullptr ) {
    boost::filesystem::path tmp( std::getenv( "SOURCE_DIR" ) );
    boost::filesystem::path::const_iterator it, it2;
    for ( it = path.begin(), it2 = tmp.begin(); it != path.end() && it2 != tmp.end(); ++it, ++it2 ) {
      if ( *it != *it2 ) {
        break;
      }
    }
    if ( it2 == tmp.end() ) {
      rt.clear();
      for ( ; it != path.end(); ++it ) {
        rt = rt / it->filename();
      }
    }
  }

  return rt;
}

void FileInfo::writeCobertura( tinyxml2::XMLElement *parent, tinyxml2::XMLDocument &doc ) const {
  tinyxml2::XMLElement *node = parent->InsertEndChild( doc.NewElement( "class" ) )->ToElement();
  node->SetAttribute( "filename", trim( source ).c_str() );

  std::string className = source.filename().string();
  size_t pos;
  while ( ( pos = className.find( '.' ) ) != std::string::npos ) {
    className[pos] = '_';
  }

  node->SetAttribute( "name", className.c_str() );

  node->InsertEndChild( doc.NewElement( "methods" ) );
  tinyxml2::XMLElement *xmllines = node->InsertEndChild( doc.NewElement( "lines" ) )->ToElement();
  for ( const auto &line : lines ) {
    line.second->writeCobertura( xmllines, doc );
  }

  size_t branches = 0, branchesHit = 0;
  for ( auto line : lines ) {
    branches += line.second->getBranchCount();
    branchesHit += line.second->getBranchesCovered();
  }

  if ( branches != 0u ) {
    node->SetAttribute( "branch-rate", double( branchesHit ) / double( branches ) );
  } else {
    node->SetAttribute( "branch-rate", 0 );
  }
  node->SetAttribute( "complexity", "0.0" );
  if ( lines.empty() ) {
    node->SetAttribute( "line-rate", 0 );
  } else {
    node->SetAttribute( "line-rate", double( getCoveredLines() ) / double( getTotalLines() ) );
  }
}

void FileInfo::cleanUp() {
  for ( auto &line : lines ) {
    line.second->cleanUp();
  }
}

const FileInfo::linesType &FileInfo::getLines() const {
  return lines;
}
