#include <boost/filesystem/fstream.hpp>
#include "TextCLIAction.h"
#include "CoverageData.h"
#include "FileInfo.h"
#include "LineInfo.h"

using namespace testing::coverage::cli;
using namespace boost::program_options;

void TextCLIAction::run() {
  loadData();
  for ( const auto &item : coverageData->getFiles() ) {
    std::cout << item->getName().string() << std::endl;
    boost::filesystem::ifstream sourceFile;
    sourceFile.open( item->getName() );
    uint32_t lineNum = 1;
    std::string sourceLine;
    for ( const auto &line : item->getLines() ) {
      while ( lineNum < line.first and not sourceFile.eof() ) {
        std::getline( sourceFile, sourceLine );
        outputLine( lineNum++, sourceLine );
      }
      std::getline( sourceFile, sourceLine );
      outputLine( lineNum++, sourceLine, line.second );
    }
    while ( not sourceFile.eof() ) {
      std::getline( sourceFile, sourceLine );
      outputLine( lineNum++, sourceLine );
    }
  }
}

std::string TextCLIAction::getName() const {
  return "Text";
}

const boost::program_options::options_description TextCLIAction::getCLIOptions() {
  auto opts = BaseInputCLIAction::getCLIOptions();
  opts.add_options()( "color", bool_switch( &color ) );
  return opts;
}

void TextCLIAction::outputLine( uint32_t num, const std::string &code, const LineInfoPtr &line ) {
  std::cout << std::setw( 5 ) << num << ":" << std::setw( 5 );
  if ( not line ) {
    std::cout << "-";
  } else {
    std::cout << line->getHits();
  }
  std::cout << " | " << code << std::endl;
}
