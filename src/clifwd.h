#pragma once
#ifndef GTESTCOVERAGE_CLIFWD_H
#define GTESTCOVERAGE_CLIFWD_H

#include <memory>

namespace testing {
  namespace coverage {
    namespace cli {
      class CLIAction;
      typedef std::shared_ptr<CLIAction> CLIActionPtr;
      class CLI;
    }
  }
}

#endif //GTESTCOVERAGE_CLIFWD_H
