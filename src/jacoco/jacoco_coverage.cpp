#include "jacoco_coverage.hpp"

CoverageSummary JacocoCoverage::buildSummary() const
{
  CoverageSummary coverageSummary;
  if (instructionMissed > 0 || instructionCovered > 0) {
    coverageSummary.instructionCoverage =
      static_cast<long double>(instructionCovered) / (instructionMissed + instructionCovered);
  }
  if (lineMissed > 0 || lineCovered > 0) {
    coverageSummary.lineCoverage = static_cast<long double>(lineCovered) / (lineMissed + lineCovered);
  }
  if (classCovered > 0 || classMissed > 0) {
    coverageSummary.classCoverage = static_cast<long double>(classCovered) / (classCovered + classMissed);
  }
  if (branchCovered > 0 || branchMissed > 0) {
    coverageSummary.branchCoverage = static_cast<long double>(branchCovered) / (branchCovered + branchMissed);
  }
  if (methodCovered > 0 || methodMissed > 0) {
    coverageSummary.methodCoverage = static_cast<long double>(methodCovered) / (methodCovered + methodMissed);
  }
  if (complexityCovered > 0 || complexityMissed > 0) {
    coverageSummary.complexityCoverage =
      static_cast<long double>(complexityCovered) / (complexityCovered + complexityMissed);
  }
  return coverageSummary;
}