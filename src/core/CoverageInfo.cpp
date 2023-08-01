#include <format>

#include "CoverageInfo.h"
void CoverageInfo::setInstructionMissed(const unsigned long value)
{
  instructionMissed = value;
}
void CoverageInfo::setInstructionCovered(const unsigned long value)
{
  instructionCovered = value;
}
void CoverageInfo::setBranchMissed(const unsigned long value)
{
  branchMissed = value;
}
void CoverageInfo::setBranchCovered(const unsigned long value)
{
  branchCovered = value;
}
void CoverageInfo::setLineMissed(const unsigned long value)
{
  lineMissed = value;
}
void CoverageInfo::setLineCovered(const unsigned long value)
{
  lineCovered = value;
}
void CoverageInfo::setMethodMissed(const unsigned long value)
{
  methodMissed = value;
}
void CoverageInfo::setMethodCovered(const unsigned long value)
{
  methodCovered = value;
}
void CoverageInfo::setClassMissed(const unsigned long value)
{
  classMissed = value;
}
void CoverageInfo::setClassCovered(const unsigned long value)
{
  classCovered = value;
}
void CoverageInfo::setComplexityMissed(const unsigned long value)
{
  complexityMissed = value;
}
void CoverageInfo::setComplexityCovered(const unsigned long value)
{
  complexityCovered = value;
}
std::string CoverageInfo::print() const
{
  std::string value{};
  if (instructionMissed > 0 || instructionCovered > 0) {
    value.append(std::format("instruction: {:.2f}\n",
      static_cast<long double>(instructionCovered) / (instructionMissed + instructionCovered)));
  }
  if (lineMissed > 0 || lineCovered > 0) {
    value.append(std::format("line: {:.2f}\n", static_cast<long double>(lineCovered) / (lineMissed + lineCovered)));
  }
  if (classMissed > 0 || classCovered > 0) {
    value.append(std::format("class: {:.2f}\n", static_cast<long double>(classCovered) / (classCovered + classMissed)));
  }
  if (branchMissed > 0 || branchCovered > 0) {
    value.append(
      std::format("branch: {:.2f}\n", static_cast<long double>(branchCovered) / (branchCovered + branchMissed)));
  }
  if (methodCovered > 0 || methodMissed > 0) {
    value.append(
      std::format("method: {:.2f}\n", static_cast<long double>(methodCovered) / (methodCovered + methodMissed)));
  }
  if (complexityCovered > 0 || complexityMissed > 0) {
    value.append(std::format(
      "complexity: {:.2f}\n", static_cast<long double>(complexityCovered) / (complexityCovered + complexityMissed)));
  }
  return value;
}
