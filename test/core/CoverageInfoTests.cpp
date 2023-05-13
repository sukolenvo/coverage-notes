#include <catch2/catch.hpp>

#include "core/CoverageInfo.h"

namespace {

TEST_CASE("CoverageInfo:all", "[core]")
{
  CoverageInfo coverageInfo;
  coverageInfo.setInstructionMissed(1);
  coverageInfo.setInstructionCovered(2);
  coverageInfo.setBranchMissed(3);
  coverageInfo.setBranchCovered(4);
  coverageInfo.setLineMissed(5);
  coverageInfo.setLineCovered(6);
  coverageInfo.setMethodMissed(7);
  coverageInfo.setMethodCovered(8);
  coverageInfo.setClassMissed(9);
  coverageInfo.setClassCovered(10);
  coverageInfo.setComplexityMissed(110);
  coverageInfo.setComplexityCovered(12);

  const auto expected = R"(instruction: 0.67
line: 0.55
class: 0.53
branch: 0.57
method: 0.53
complexity: 0.1
)";
  REQUIRE(coverageInfo.print() == expected);
}

TEST_CASE("CoverageInfo:no_info", "[core]")
{
  CoverageInfo coverageInfo;

  REQUIRE(coverageInfo.print() == "");
}

TEST_CASE("CoverageInfo:full_coverage", "[core]")
{
  CoverageInfo coverageInfo;
  coverageInfo.setInstructionCovered(2);
  coverageInfo.setBranchCovered(4);
  coverageInfo.setLineCovered(6);
  coverageInfo.setMethodCovered(8);
  coverageInfo.setClassCovered(10);
  coverageInfo.setComplexityCovered(12);

  const auto expected = R"(instruction: 1.00
line: 1.00
class: 1.00
branch: 1.00
method: 1.00
complexity: 1.00
)";
  REQUIRE(coverageInfo.print() == expected);
}

TEST_CASE("CoverageInfo:no_coverage", "[core]")
{
  CoverageInfo coverageInfo;
  coverageInfo.setInstructionMissed(1);
  coverageInfo.setBranchMissed(3);
  coverageInfo.setLineMissed(5);
  coverageInfo.setMethodMissed(7);
  coverageInfo.setClassMissed(9);
  coverageInfo.setComplexityMissed(110);

  const auto expected = R"(instruction: 0.00
line: 0.00
class: 0.00
branch: 0.00
method: 0.00
complexity: 0.00
)";
  REQUIRE(coverageInfo.print() == expected);
}

} // namespace