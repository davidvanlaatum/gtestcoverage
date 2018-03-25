#pragma once
#ifndef GTESTCOVERAGE_SUMMARYCLIACTION_H
#define GTESTCOVERAGE_SUMMARYCLIACTION_H

#include "BaseInputCLIAction.h"

namespace testing {
  namespace coverage {
    namespace cli {
      class SummaryCLIAction : public BaseInputCLIAction {
      public:
        void run() override;
        std::string getName() const override;
      };
    }
  }
}

#endif //GTESTCOVERAGE_SUMMARYCLIACTION_H
