#ifndef COVERAGE_NOTES_COVERAGEINFO_H
#define COVERAGE_NOTES_COVERAGEINFO_H

#include <string>

class CoverageInfo
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
  void setInstructionMissed(unsigned long value);
  void setInstructionCovered(unsigned long value);
  void setBranchMissed(unsigned long value);
  void setBranchCovered(unsigned long value);
  void setLineMissed(unsigned long value);
  void setLineCovered(unsigned long value);
  void setMethodMissed(unsigned long value);
  void setMethodCovered(unsigned long value);
  void setClassMissed(unsigned long value);
  void setClassCovered(unsigned long value);
  void setComplexityMissed(unsigned long value);
  void setComplexityCovered(unsigned long value);
  [[nodiscard]] std::string print() const;
  auto operator <=>(const CoverageInfo& other) const = default;
};
#endif // COVERAGE_NOTES_COVERAGEINFO_H
