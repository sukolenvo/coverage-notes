#ifndef COVERAGE_NOTES_COVERAGE_SUMMARY_HPP
#define COVERAGE_NOTES_COVERAGE_SUMMARY_HPP

#include <ostream>
#include <string>

#include "coverage_info.hpp"

class CoverageSummary
{
  long double instructionCoverage{};
  long double lineCoverage{};
  long double classCoverage{};
  long double branchCoverage{};
  long double methodCoverage{};
  long double complexityCoverage{};

public:
  explicit CoverageSummary(const std::string &printed);
  explicit CoverageSummary(const CoverageInfo &coverageInfo);

  [[nodiscard]] std::string print() const;
  [[nodiscard]] bool empty() const;
  [[nodiscard]] std::string diff(const CoverageSummary &other) const;
  auto operator<=>(const CoverageSummary &other) const noexcept = default;
};

std::ostream &operator<<(std::ostream &stream, const CoverageSummary &v);

#endif // COVERAGE_NOTES_COVERAGE_SUMMARY_HPP
