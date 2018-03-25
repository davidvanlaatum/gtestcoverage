#pragma once
#ifndef GTESTCOVERAGE_CLIACTION_H
#define GTESTCOVERAGE_CLIACTION_H

#include "clifwd.h"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>

namespace testing {
  namespace coverage {
    namespace cli {
      class CLIAction {
      public:
        virtual ~CLIAction() = default;
        virtual const boost::program_options::options_description getCLIOptions() = 0;
        virtual void getCLIPositional( boost::program_options::positional_options_description &positional ) = 0;
        virtual void run() = 0;
        virtual std::string getName() const = 0;
      };
    }
  }
}

#endif //GTESTCOVERAGE_CLIACTION_H
