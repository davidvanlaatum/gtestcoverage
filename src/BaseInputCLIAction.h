#pragma once
#ifndef GTESTCOVERAGE_BASEINPUTCLIACTION_H
#define GTESTCOVERAGE_BASEINPUTCLIACTION_H

#include "CLIAction.h"
#include "fwd.h"
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

namespace testing {
  namespace coverage {
    namespace cli {
      class BaseInputCLIAction : public CLIAction {
      public:
        const boost::program_options::options_description getCLIOptions() override;
        void getCLIPositional( boost::program_options::positional_options_description &positional ) override;
      protected:
        boost::optional<boost::filesystem::path> input;
        CoverageDataPtr coverageData;

        void loadData();
      };
    }
  }
}

#endif //GTESTCOVERAGE_BASEINPUTCLIACTION_H
