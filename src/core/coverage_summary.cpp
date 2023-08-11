#include <cmath>
#include <string>

#include <spdlog/spdlog.h>

#include "coverage_summary.hpp"

CoverageSummary::CoverageSummary(const std::string &printed)
{
  size_t start = 0;
  for (auto end = printed.find('\n', start); end != std::string::npos;
       start = end + 1, end = printed.find('\n', start)) {
    auto separatorIdx = printed.find(':', start);
    if (separatorIdx != std::string::npos && separatorIdx < end) {
      std::string type = printed.substr(start, separatorIdx - start);
      std::string value = printed.substr(separatorIdx + 2, end - separatorIdx - 2);
      long double coverage = std::stold(value);
      if (coverage <= 0 || coverage > 1) {
        spdlog::warn("Invalid coverage {} in {}", value, printed);
        continue;
      }
      if (type == "instruction") {
        instructionCoverage = coverage;
      } else if (type == "line") {
        lineCoverage = coverage;
      } else if (type == "class") {
        classCoverage = coverage;
      } else if (type == "branch") {
        branchCoverage = coverage;
      } else if (type == "method") {
        methodCoverage = coverage;
      } else if (type == "complexity") {
        complexityCoverage = coverage;
      } else {
        spdlog::warn("Invalid coverage type {} in {}", type, printed);
      }
    }
  }
}

CoverageSummary::CoverageSummary(const CoverageInfo &coverageInfo)
{
  if (coverageInfo.instructionMissed > 0 || coverageInfo.instructionCovered > 0) {
    instructionCoverage = static_cast<long double>(coverageInfo.instructionCovered)
                          / (coverageInfo.instructionMissed + coverageInfo.instructionCovered);
  }
  if (coverageInfo.lineMissed > 0 || coverageInfo.lineCovered > 0) {
    lineCoverage =
      static_cast<long double>(coverageInfo.lineCovered) / (coverageInfo.lineMissed + coverageInfo.lineCovered);
  }
  if (coverageInfo.classCovered > 0 || coverageInfo.classMissed > 0) {
    classCoverage =
      static_cast<long double>(coverageInfo.classCovered) / (coverageInfo.classCovered + coverageInfo.classMissed);
  }
  if (coverageInfo.branchCovered > 0 || coverageInfo.branchMissed > 0) {
    branchCoverage =
      static_cast<long double>(coverageInfo.branchCovered) / (coverageInfo.branchCovered + coverageInfo.branchMissed);
  }
  if (coverageInfo.methodCovered > 0 || coverageInfo.methodMissed > 0) {
    methodCoverage =
      static_cast<long double>(coverageInfo.methodCovered) / (coverageInfo.methodCovered + coverageInfo.methodMissed);
  }
  if (coverageInfo.complexityCovered > 0 || coverageInfo.complexityMissed > 0) {
    complexityCoverage = static_cast<long double>(coverageInfo.complexityCovered)
                         / (coverageInfo.complexityCovered + coverageInfo.complexityMissed);
  }
}

std::string CoverageSummary::print() const
{
  std::string value{};
  if (instructionCoverage > 0) {
    value.append(fmt::format("instruction: {:.2f}\n", instructionCoverage));
  }
  if (lineCoverage > 0) {
    value.append(fmt::format("line: {:.2f}\n", lineCoverage));
  }
  if (classCoverage > 0) {
    value.append(fmt::format("class: {:.2f}\n", classCoverage));
  }
  if (branchCoverage > 0) {
    value.append(fmt::format("branch: {:.2f}\n", branchCoverage));
  }
  if (methodCoverage > 0) {
    value.append(fmt::format("method: {:.2f}\n", methodCoverage));
  }
  if (complexityCoverage > 0) {
    value.append(fmt::format("complexity: {:.2f}\n", complexityCoverage));
  }
  return value;
}

bool CoverageSummary::empty() const
{
  return instructionCoverage < 0.01 && lineCoverage < 0.01 && classCoverage < 0.01 && branchCoverage < 0.01
         && methodCoverage < 0.01 && complexityCoverage < 0.01;
}

std::string formatDiff(const std::string &type, long double base, long double head)
{
  int basePercent = static_cast<int>(std::roundl(base * 100));
  int headPercent = static_cast<int>(std::roundl(head * 100));
  if (basePercent == headPercent) {
    return fmt::format("  {}: {}%\n", type, basePercent);
  }
  if (basePercent > headPercent) {
    return fmt::format("- {}: {}% ({})\n", type, headPercent, headPercent - basePercent);
  }
  return fmt::format("+ {}: {}% (+{})\n", type, headPercent, headPercent - basePercent);
}

std::string CoverageSummary::diff(const CoverageSummary &other) const
{
  std::string result = "```diff\n@@ Coverage info. Don't edit past this section @@\n";
  result += formatDiff("instruction", instructionCoverage, other.instructionCoverage);
  result += formatDiff("line", lineCoverage, other.lineCoverage);
  result += formatDiff("class", classCoverage, other.classCoverage);
  result += formatDiff("branch", branchCoverage, other.branchCoverage);
  result += formatDiff("method", methodCoverage, other.methodCoverage);
  result += formatDiff("complexity", complexityCoverage, other.complexityCoverage);
  result += "```";
  return result;
}

std::ostream &operator<<(std::ostream &stream, const CoverageSummary &v)
{
  if (stream) {
    stream << v.print();
  }
  return stream;
}
