#ifndef COVERAGE_NOTES_COBERTURA_COVERAGE_HPP
#define COVERAGE_NOTES_COBERTURA_COVERAGE_HPP

#include "core/coverage_summary.hpp"

struct CoberturaCoverage
{
  long double lineCoverage{};
  long double branchCoverage{};

  [[nodiscard]] CoverageSummary buildSummary() const;
};

#endif // COVERAGE_NOTES_COBERTURA_COVERAGE_HPP
