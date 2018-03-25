#include "CLI.h"
#include "CoverageData.h"
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <string>
#include <boost/algorithm/string/predicate.hpp>
#include <sysexits.h>
#include <fstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <json.hpp>
#include <boost/filesystem/operations.hpp>
#include <stdserializers.h>
#include "CoberturaCLIAction.h"
#include "SummaryCLIAction.h"
#include "TextCLIAction.h"
#include <boost/algorithm/string/case_conv.hpp>

using namespace testing::coverage::cli;
using namespace boost::program_options;

void testing::coverage::cli::validate( boost::any &v, const std::vector<std::string> &values, CLI::Mode *, int ) {
  validators::check_first_occurrence( v );
  const std::string &s = validators::get_single_string( values );
  if ( boost::iequals( s, "cobertura" ) ) {
    v = CLI::Cobertura;
  } else if ( boost::iequals( s, "summary" ) ) {
    v = CLI::Summary;
  } else if ( boost::iequals( s, "html" ) ) {
    v = CLI::HTML;
  } else if ( boost::iequals( s, "text" ) ) {
    v = CLI::Text;
  } else {
    throw validation_error( validation_error::invalid_option_value );
  }
}

CLI::CLI() {
  cliOptions.add_options()
                ( "version,v", bool_switch( &version ), "print version string" )
                ( "help,h", bool_switch( &help ), "produce help message" );
  actions[Cobertura] = std::make_shared<CoberturaCLIAction>();
  actions[Summary] = std::make_shared<SummaryCLIAction>();
  actions[Text] = std::make_shared<TextCLIAction>();
}

int CLI::run( int argc, char **argv ) {
  executable = argv[0];
  baseName = boost::filesystem::path( executable ).filename();
  parseArgs( argc, argv );
  doVersion();
  doHelp();
  if ( code == OK && action ) {
    try {
      action->run();
    } catch ( CLIException &e ) {
      std::clog << e.what() << std::endl;
      code = e.getCode();
    } catch ( std::exception &e ) {
      std::clog << e.what() << std::endl;
      code = Failure;
    }
  }
  return code;
}

void CLI::doUsage( const CLIActionPtr &ptr ) {
  std::clog << "Usage: " << baseName.string() << " [options]";
  std::clog << " " << boost::to_lower_copy( ptr->getName() );
  positional_options_description pos;
  ptr->getCLIPositional( pos );
  for ( unsigned i = 0; i < pos.max_total_count(); i++ ) {
    std::clog << " <" << pos.name_for_position( i ) << ">";
  }
  std::clog << std::endl;
}

void CLI::doHelp() {
  if ( help ) {
    options_description all;
    all.add( cliOptions );
    for ( const auto &item : actions ) {
      doUsage( item.second );
      all.add( item.second->getCLIOptions() );
    }
    std::clog << all << std::endl;
    code = Usage;
  }
}

void CLI::doVersion() {
  if ( version ) {
    std::clog << "GTestCoverage version: " << VERSION_STR << std::endl;
    code = Usage;
  }
}

void CLI::parseArgs( int argc, char **argv ) {
  variables_map vm;
  try {
    options_description all;
    all.add( cliOptions );
    all.add_options()( "unnamed", value<std::vector<std::string>>()->multitoken() );
    all.add_options()( "mode", value( &mode ) );
    {
      boost::program_options::positional_options_description cliPositional;
      cliPositional.add( "mode", 1 );
      cliPositional.add( "unnamed", -1 );
      auto parsed = command_line_parser( argc, argv )
          .allow_unregistered()
          .options( all )
          .positional( cliPositional )
          .run();
      store( parsed, vm );
      notify( vm );
    }
    if ( mode != Unset ) {
      boost::program_options::positional_options_description cliPositional;
      cliPositional.add( "mode", 1 );
      auto p = actions.find( mode );
      if ( p != actions.end() ) {
        action = p->second;
      } else {
        throw CLIException( Usage, "No driver for mode" );
      }
      action->getCLIPositional( cliPositional );
      all.add( action->getCLIOptions() );
      auto parsed = command_line_parser( argc, argv )
          .allow_unregistered()
          .options( all )
          .positional( cliPositional )
          .run();
      store( parsed, vm );
      notify( vm );
    } else {
      help = true;
    }
  } catch ( CLIException &e ) {
    std::clog << e.what() << std::endl;
    code = e.getCode();
  } catch ( std::exception &e ) {
    std::clog << e.what() << std::endl;
    help = true;
  }
}
