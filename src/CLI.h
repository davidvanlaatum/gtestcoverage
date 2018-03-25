#pragma once
#ifndef GTESTCOVERAGE_CLI_H
#define GTESTCOVERAGE_CLI_H

#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <sysexits.h>
#include "fwd.h"
#include "clifwd.h"
#include "CLIException.h"

namespace testing {
  namespace coverage {
    namespace cli {
      class CLI {
      public:
        CLI();
        int run( int argc, char **argv );

        enum Mode {
          Unset,
          Summary,
          Cobertura,
          HTML,
          Text
        };

      protected:
        boost::program_options::options_description cliOptions;
        Mode mode{ Unset };
        bool help{ false };
        bool version{ false };
        ExitCode code{ OK };
        std::string executable;
        boost::filesystem::path baseName;
        std::map<Mode, CLIActionPtr> actions;
        CLIActionPtr action;

        void doUsage( const CLIActionPtr &ptr );
        void doHelp();
        void doVersion();
        void parseArgs( int argc, char **argv );
      };
      void validate( boost::any &v, const std::vector<std::string> &values, CLI::Mode *, int );
    }
  }
}

#endif //GTESTCOVERAGE_CLI_H
