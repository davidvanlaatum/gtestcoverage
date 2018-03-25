#pragma once
#ifndef GTESTCOVERAGE_BASEINPUTOUTPUTCLIACTION_H
#define GTESTCOVERAGE_BASEINPUTOUTPUTCLIACTION_H

#include "BaseInputCLIAction.h"

namespace testing {
  namespace coverage {
    namespace cli {
      class BaseInputOutputCLIAction : public BaseInputCLIAction {
      public:
        const boost::program_options::options_description getCLIOptions() override;
        void getCLIPositional( boost::program_options::positional_options_description &positional ) override;
      protected:
        boost::filesystem::path output;
      };
    }
  }
}

#endif //GTESTCOVERAGE_BASEINPUTOUTPUTCLIACTION_H
