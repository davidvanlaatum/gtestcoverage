#include "CoverageData.h"
#include <boost/make_shared.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <tinyxml2.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace testing::coverage;
using namespace std;
using namespace boost::filesystem;

namespace std {
  template<class X, class Y> ostream &operator<<( ostream &os, const map<X, Y> &data ) {
    os << "[" << endl;
    for ( typename map<X, Y>::const_iterator it = data.begin(); it != data.end(); ++it ) {
      os << it->first << " = " << it->second << endl;
    }
    os << "]" << endl;
    return os;
  }

  template<class T> ostream &operator<<( ostream &os, const vector<T> &data ) {
    os << "[";
    for ( typename vector<T>::const_iterator it = data.begin(); it != data.end(); ++it ) {
      if ( it != data.begin() ) { os << ","; }
      os << *it;
    }
    os << "]";
    return os;
  }
}  // namespace std

namespace boost {
  template<class T> ostream &operator<<( ostream &os, const boost::shared_ptr<T> &ptr ) {
    if ( ptr ) {
      os << *ptr.get();
    } else {
      os << "NULL";
    }
    return os;
  }
} // namespace boost

void CoverageData::addFile( CoverageData::path file ) {
  files[file] = boost::make_shared<FileInfo>( file );
}

void CoverageData::addCoversDir( const CoverageData::path &dir ) {
  coveredDirs.push_back( dir );
}

void CoverageData::addCovers( path file ) {
  bool found = false;
  for ( auto &it : files ) {
    if ( it.first.filename() == file ) {
      found = true;
      file = it.first;
      it.second->setExplicitCovers();
      break;
    }
  }
  if ( found ) {
    coveredFiles.push_back( file );
  } else if ( !files.empty() ) {
    GTEST_LOG_( ERROR ) << "Failed to find " << file << " in sources";
  }
}

void CoverageData::setOutputFile( const path &file ) {
  outputFile = file;
}

TestInfo::Ptr CoverageData::loadTestData( const std::string &suite, const std::string &name, bool passed ) {
  TestInfo::Ptr test = boost::make_shared<TestInfo>( suite, name, coveredFiles, passed );
  tests.push_back( test );
  for ( auto &coverageFile : coverageFiles ) {
    if ( exists( coverageFile ) ) {
      processGCDAFile( test, coverageFile );
      remove( coverageFile );
    }
  }
  return test;
}

void CoverageData::resolveFiles() {
  for ( auto &coveredDir : coveredDirs ) {
    directory_iterator dir( coveredDir );
    while ( dir != directory_iterator() ) {
      if ( dir->path().extension() == ".gcda" ) {
        attachGCDAFile( dir->path() );
        remove( dir->path() );
      }
      dir++;
    }
  }
}

double toPercent( size_t count, size_t total ) {
  double rt = 0;

  if ( total > 0 ) {
    rt = ( double( count ) * 100.0 ) / double( total );
  }

  return rt;
}

void CoverageData::dumpStats() {
  if ( !outputFile.empty() ) {
    writeOutputFile();
    GTEST_LOG_( INFO ) << "Wrote coverage to " << outputFile;
  }
  size_t coveredLines = 0, totalLines = 0, explicitCoveredLines = 0, explicitTotalLines = 0;
  for ( auto &it : files ) {
    boost::shared_ptr<FileInfo> &file = it.second;
    if ( file->isExplicitCovered() ) {
      explicitTotalLines += file->getTotalLines();
      explicitCoveredLines += file->getCoveredLines();
    }
    coveredLines += file->getCoveredLines();
    totalLines += file->getTotalLines();
  }
  GTEST_LOG_( INFO ) << std::setprecision( 3 )
                     << "Covered " << coveredLines << " of " << totalLines << " ("
                     << toPercent( coveredLines, totalLines ) << "%)"
                     << " (" << explicitCoveredLines << " of " << explicitTotalLines << " explicit ("
                     << toPercent( explicitCoveredLines, explicitTotalLines ) << "%))";
}

void CoverageData::attachGCDAFile( const path &file ) {
  if ( processGCDAFile( TestInfo::Ptr(), file ) ) {
    coverageFiles.push_back( file );
  }
}

bool CoverageData::processGCDAFile( const TestInfo::Ptr &test, const path &file ) {
  bool rt = false;
  int odir = open( ".", O_RDONLY );
  chdir( getenv( "CMAKE_BINARY_DIR" ) );
  string cmd = getenv( "GCOV" ) + string( " -bcfa " ) + file.native();
  FILE *gcov = popen( cmd.c_str(), "r" );
  if ( gcov != nullptr ) {
    char buffer[1024];
    while ( fgets( buffer, sizeof( buffer ), gcov ) != nullptr ) {}
    int ex = pclose( gcov );
    if ( ex != 0 ) {
      GTEST_LOG_( ERROR ) << "gcov returned " << ex;
    }
  }
  for ( directory_iterator dir( current_path() ); dir != directory_iterator(); ++dir ) {
    if ( dir->path().extension() == ".gcov" ) {
      if ( processGCovFile( test, dir->path() ) ) { rt = true; }
      remove( dir->path() );
    }
  }
  fchdir( odir );
  close( odir );
  return rt;
}

bool CoverageData::processGCovFile( const TestInfo::Ptr &test, const path &file ) {
  bool rt = false;
  std::ifstream data( file.string().c_str(), std::ios_base::in );
  std::string line;
  std::getline( data, line );
  size_t pos = line.find( "Source:" );
  if ( pos != std::string::npos ) {
    auto fileName = boost::filesystem::canonical( line.substr( pos + 7 ) );
    auto it = files.find( fileName );
    if ( it != files.end() && isInteresting( it->first ) ) {
      data.seekg( 0 );
      it->second->processFile( test, data );
      rt = true;
    }
  }
  return rt;
}

bool CoverageData::isInteresting( const path &path ) {
  bool rt = false;
  if ( coveredFiles.empty() ) { rt = true; }
  else {
    for ( auto &coveredFile : coveredFiles ) {
      if ( coveredFile == path ) {
        rt = true;
        break;
      }
    }
  }
  return rt;
}

void CoverageData::beginNewTest() {
  coveredFiles.clear();
}

void CoverageData::writeOutputFile() {
  cleanUp();
  writeCobertura();
}

using namespace tinyxml2;

void CoverageData::writeCobertura() const {
  XMLDocument doc;
  doc.InsertEndChild( doc.NewDeclaration( nullptr ) );
  doc.InsertEndChild(
      doc.NewUnknown( R"(DOCTYPE coverage SYSTEM "http://cobertura.sourceforge.net/xml/coverage-04.dtd")" ) );
  XMLElement *root = doc.InsertEndChild( doc.NewElement( "coverage" ) )->ToElement();
  root->SetAttribute( "timestamp", static_cast<int>( time( nullptr )) );
  root->SetAttribute( "version", "Something" );

  XMLElement *sources = root->InsertEndChild( doc.NewElement( "sources" ) )->ToElement();
  if ( std::getenv( "SOURCE_DIR" ) != nullptr ) {
    sources->InsertEndChild( doc.NewElement( "source" ) )
           ->InsertEndChild( doc.NewText( std::getenv( "SOURCE_DIR" ) ) );
  }

  XMLElement *packages = root->InsertEndChild( doc.NewElement( "packages" ) )->ToElement();
  XMLElement *package = packages->InsertEndChild( doc.NewElement( "package" ) )->ToElement();
  package->SetAttribute( "name", std::getenv( "COVERS" ) );

  XMLElement *classes = package->InsertEndChild( doc.NewElement( "classes" ) )->ToElement();

  size_t totalLines = 0, coveredLines = 0;

  for ( const auto &file : files ) {
    file.second->writeCobertura( classes, doc );
    totalLines += file.second->getTotalLines();
    coveredLines += file.second->getCoveredLines();
  }

  package->SetAttribute( "branch-rate", 0 );
  package->SetAttribute( "complexity", "0.0" );
  package->SetAttribute( "line-rate", double( coveredLines ) / double( totalLines ) );
  root->SetAttribute( "branch-rate", 0 );
  root->SetAttribute( "line-rate", double( coveredLines ) / double( totalLines ) );

  doc.SaveFile( outputFile.c_str() );
}

void CoverageData::cleanUp() {
  for ( auto it = files.begin(); it != files.end(); ) {
//    if ( it->second->getTotalLines() == 0 ) {
//      it = files.erase( it );
//    } else {
      it->second->cleanUp();
      ++it;
//    }
  }
}
