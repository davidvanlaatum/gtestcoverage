#include "SummaryCLIAction.h"
#include "CoverageData.h"

using namespace testing::coverage::cli;

void SummaryCLIAction::run() {
  loadData();
  coverageData->printSummary( std::cout );
}

std::string SummaryCLIAction::getName() const {
  return "Summary";
}
