#include "BaseInputCLIAction.h"
#include <json.hpp>
#include <stdserializers.h>
#include <boost/filesystem.hpp>
#include "CoverageData.h"
#include "CLIException.h"

using namespace testing::coverage::cli;
using namespace boost::program_options;

const options_description BaseInputCLIAction::getCLIOptions() {
  options_description args( getName() );
  args.add_options()( "input", value( &input )->value_name( "file" ), "input file" );
  return args;
}

void BaseInputCLIAction::getCLIPositional( positional_options_description &positional ) {
  positional.add( "input", 1 );
}

void BaseInputCLIAction::loadData() {
  if ( boost::filesystem::exists( input.value() ) ) {
    boost::filesystem::ifstream inputFile( input.value(), std::ios_base::in );
    nlohmann::json data;
    inputFile >> data;
    coverageData = data;
  } else {
    std::ostringstream str;
    str << "File: " << input.get() << " does not exist" << std::endl;
    throw CLIException( NoInput, str.str() );
  }
}
