#pragma once
#ifndef GTESTCOVERAGE_TEXTCLIACTION_H
#define GTESTCOVERAGE_TEXTCLIACTION_H

#include "fwd.h"
#include "BaseInputCLIAction.h"

namespace testing {
  namespace coverage {
    namespace cli {
      class TextCLIAction : public BaseInputCLIAction {
      public:
        void run() override;
        std::string getName() const override;
        const boost::program_options::options_description getCLIOptions() override;
      protected:
        bool color{ false };
        void outputLine( uint32_t num, const std::string &code, const LineInfoPtr &line = LineInfoPtr() );
      };
    }
  }
}

#endif //GTESTCOVERAGE_TEXTCLIACTION_H
