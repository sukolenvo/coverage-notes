#ifndef COVERAGE_NOTES_COVERAGE_INFO_HPP
#define COVERAGE_NOTES_COVERAGE_INFO_HPP

#include <string>

struct CoverageInfo
{
  unsigned long instructionMissed{};
  unsigned long instructionCovered{};
  unsigned long branchMissed{};
  unsigned long branchCovered{};
  unsigned long lineMissed{};
  unsigned long lineCovered{};
  unsigned long methodMissed{};
  unsigned long methodCovered{};
  unsigned long classMissed{};
  unsigned long classCovered{};
  unsigned long complexityMissed{};
  unsigned long complexityCovered{};

public:
  auto operator<=>(const CoverageInfo &other) const = default;
};
#endif // COVERAGE_NOTES_COVERAGE_INFO_HPP
