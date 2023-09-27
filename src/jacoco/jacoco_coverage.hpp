#ifndef COVERAGE_NOTES_JACOCO_COVERAGE_HPP
#define COVERAGE_NOTES_JACOCO_COVERAGE_HPP

#include <string>

#include "core/coverage_summary.hpp"

struct JacocoCoverage
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
  auto operator<=>(const JacocoCoverage &other) const = default;

  [[nodiscard]] CoverageSummary buildSummary() const;
};
#endif // COVERAGE_NOTES_JACOCO_COVERAGE_HPP
