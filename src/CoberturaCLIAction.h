#pragma once
#ifndef GTESTCOVERAGE_COBERTURACLIACTION_H
#define GTESTCOVERAGE_COBERTURACLIACTION_H

#include "BaseInputOutputCLIAction.h"

namespace testing {
  namespace coverage {
    namespace cli {
      class CoberturaCLIAction : public BaseInputOutputCLIAction {
      public:
        void run() override;
        std::string getName() const override;
      };
    }
  }
}

#endif //GTESTCOVERAGE_COBERTURACLIACTION_H
