#include <catch2/catch_test_macros.hpp>

#include "core/coverage_summary.hpp"
#include "jacoco/jacoco_coverage.hpp"

namespace {

TEST_CASE("CoverageSummary:all", "[core]")
{
  JacocoCoverage coverageInfo;
  coverageInfo.instructionMissed = 1;
  coverageInfo.instructionCovered = 2;
  coverageInfo.branchMissed = 3;
  coverageInfo.branchCovered = 4;
  coverageInfo.lineMissed = 5;
  coverageInfo.lineCovered = 6;
  coverageInfo.methodMissed = 7;
  coverageInfo.methodCovered = 8;
  coverageInfo.classMissed = 9;
  coverageInfo.classCovered = 10;
  coverageInfo.complexityMissed = 68;
  coverageInfo.complexityCovered = 12;
  const CoverageSummary summary = coverageInfo.buildSummary();

  const auto expected = R"(instruction: 0.67
line: 0.55
class: 0.53
branch: 0.57
method: 0.53
complexity: 0.15
)";
  REQUIRE(summary.print() == expected);
  REQUIRE(CoverageSummary{ summary.print() }.print() == expected);
}

TEST_CASE("CoverageSummary:diff", "[core]")
{
  CoverageSummary base{ R"(instruction: 0.67
line: 0.55
class: 0.53
branch: 0.57
method: 0.53
complexity: 0.15
)" };
  CoverageSummary head{ R"(instruction: 0.68
line: 0.45
class: 0.53
branch: 0.57
method: 0.53
complexity: 0.15
)" };

  REQUIRE(base.diff(head) == R"(```diff
@@ Coverage info. Don't edit past this section @@
+ instruction: 68% (+1)
- line: 45% (-10)
  class: 53%
  branch: 57%
  method: 53%
  complexity: 15%
```)");
}

TEST_CASE("CoverageSummary:empty", "[core]")
{
  CoverageSummary summary{ R"(instruction: 0.67
line: 0.55
class: 0.53
branch: 0.57
method: 0.53
complexity: 0.15
)" };
  CoverageSummary empty = JacocoCoverage{}.buildSummary();

  REQUIRE(summary.empty() == false);
  REQUIRE(empty.empty());
}

TEST_CASE("CoverageSummary:full_coverage", "[core]")
{
  JacocoCoverage coverageInfo;
  coverageInfo.instructionCovered = 2;
  coverageInfo.branchCovered = 4;
  coverageInfo.lineCovered = 6;
  coverageInfo.methodCovered = 8;
  coverageInfo.classCovered = 10;
  coverageInfo.complexityCovered = 12;
  CoverageSummary summary = coverageInfo.buildSummary();

  const auto expected = R"(instruction: 1.00
line: 1.00
class: 1.00
branch: 1.00
method: 1.00
complexity: 1.00
)";
  REQUIRE(summary.print() == expected);
}

TEST_CASE("CoverageSummary:no_coverage", "[core]")
{
  JacocoCoverage coverageInfo;
  coverageInfo.instructionMissed = 1;
  coverageInfo.branchMissed = 3;
  coverageInfo.lineMissed = 5;
  coverageInfo.methodMissed = 7;
  coverageInfo.classMissed = 9;
  coverageInfo.complexityMissed = 110;
  CoverageSummary summary = coverageInfo.buildSummary();

  REQUIRE(summary.print().empty());
}

} // namespace