#include "cobertura_coverage.hpp"

CoverageSummary CoberturaCoverage::buildSummary() const
{
  CoverageSummary coverageSummary;
  coverageSummary.lineCoverage = lineCoverage;
  coverageSummary.branchCoverage = branchCoverage;
  return coverageSummary;
}