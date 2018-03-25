#include "BaseInputOutputCLIAction.h"

using namespace testing::coverage::cli;
using namespace boost::program_options;

const boost::program_options::options_description BaseInputOutputCLIAction::getCLIOptions() {
  auto opts = BaseInputCLIAction::getCLIOptions();
  opts.add_options()( "output", value( &output )->value_name( "file" ), "output file" );
  return opts;
}

void BaseInputOutputCLIAction::getCLIPositional( positional_options_description &positional ) {
  BaseInputCLIAction::getCLIPositional( positional );
  positional.add( "output", 1 );
}
