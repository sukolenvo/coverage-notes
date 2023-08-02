#include <catch2/catch_test_macros.hpp>
#include <simple_cpp_xml/exception.hpp>

#include "jacoco/JacocoParser.h"

namespace {

TEST_CASE("JacocoParserTests:all", "[jacoco]")
{
  // language=XML
  const auto xml =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?><!DOCTYPE report PUBLIC "-//JACOCO//DTD Report 1.1//EN" "report.dtd">
<report name="test-service">
  <sessioninfo id="object-21061aa9" start="1683895529957" dump="1683895566710"/>
  <counter type="INSTRUCTION" missed="1" covered="2"/>
  <counter type="BRANCH" missed="3" covered="4"/>
  <counter type="LINE" missed="5" covered="6"/>
  <counter type="COMPLEXITY" missed="7" covered="8"/>
  <counter type="METHOD" missed="9" covered="10"/>
  <counter type="CLASS" missed="11" covered="12"/>
</report>
)";
  JacocoParser parser;
  parser.parse(xml);
  CoverageInfo expected;
  expected.setInstructionMissed(1);
  expected.setInstructionCovered(2);
  expected.setBranchMissed(3);
  expected.setBranchCovered(4);
  expected.setLineMissed(5);
  expected.setLineCovered(6);
  expected.setComplexityMissed(7);
  expected.setComplexityCovered(8);
  expected.setMethodMissed(9);
  expected.setMethodCovered(10);
  expected.setClassMissed(11);
  expected.setClassCovered(12);
  REQUIRE(parser.getCoverageInfo() == expected);
}

TEST_CASE("JacocoParserTests:malformed", "[jacoco]")
{
  // language=XML
  const auto xml =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?><!DOCTYPE report PUBLIC "-//JACOCO//DTD Report 1.1//EN" "report.dtd">
<report name="test-service">
<test>
</report>
)";
  JacocoParser parser;
  REQUIRE_THROWS_AS(parser.parse(xml), simple_cpp::xml::ParseError);
}

TEST_CASE("JacocoParserTests:unrecognized_counter", "[jacoco]")
{
  // language=XML
  const auto xml =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?><!DOCTYPE report PUBLIC "-//JACOCO//DTD Report 1.1//EN" "report.dtd">
<report name="test-service">
<counter type="TEST" missed="5" covered="6"/>
<counter type="INSTRUCTION" missed="1" covered="2"/>
</report>
)";
  JacocoParser parser;
  parser.parse(xml);
  CoverageInfo expected;
  expected.setInstructionMissed(1);
  expected.setInstructionCovered(2);
  REQUIRE(parser.getCoverageInfo() == expected);
}

TEST_CASE("JacocoParserTests:negative_counter", "[jacoco]")
{
  // language=XML
  const auto xml =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?><!DOCTYPE report PUBLIC "-//JACOCO//DTD Report 1.1//EN" "report.dtd">
<report name="test-service">
<counter type="INSTRUCTION" missed="-1" covered="2"/>
</report>
)";
  JacocoParser parser;
  REQUIRE_THROWS_AS(parser.parse(xml), simple_cpp::xml::ParseError);
}

TEST_CASE("JacocoParserTests:parse_integer_error", "[jacoco]")
{
  // language=XML
  const auto xml =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?><!DOCTYPE report PUBLIC "-//JACOCO//DTD Report 1.1//EN" "report.dtd">
<report name="test-service">
<counter type="INSTRUCTION" missed="test" covered="2"/>
</report>
)";
  JacocoParser parser;
  REQUIRE_THROWS_AS(parser.parse(xml), simple_cpp::xml::ParseError);
}

TEST_CASE("JacocoParserTests:unrecognised_attribute", "[jacoco]")
{
  // language=XML
  const auto xml =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?><!DOCTYPE report PUBLIC "-//JACOCO//DTD Report 1.1//EN" "report.dtd">
<report name="test-service">
<counter type="INSTRUCTION" missed="1" covered="2" other="3"/>
</report>
)";
  JacocoParser parser;
  parser.parse(xml);
  CoverageInfo expected;
  expected.setInstructionMissed(1);
  expected.setInstructionCovered(2);
  REQUIRE(parser.getCoverageInfo() == expected);
}

TEST_CASE("JacocoParserTests:no_missing", "[jacoco]")
{
  // language=XML
  const auto xml =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?><!DOCTYPE report PUBLIC "-//JACOCO//DTD Report 1.1//EN" "report.dtd">
<report name="test-service">
<counter type="INSTRUCTION" covered="2"/>
</report>
)";
  JacocoParser parser;
  parser.parse(xml);
  CoverageInfo expected;
  expected.setInstructionCovered(2);
  REQUIRE(parser.getCoverageInfo() == expected);
}

TEST_CASE("JacocoParserTests:no_values", "[jacoco]")
{
  // language=XML
  const auto xml =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?><!DOCTYPE report PUBLIC "-//JACOCO//DTD Report 1.1//EN" "report.dtd">
<report name="test-service">
<counter type="INSTRUCTION"/>
</report>
)";
  JacocoParser parser;
  parser.parse(xml);
  CoverageInfo expected;
  REQUIRE(parser.getCoverageInfo() == expected);
}

} // namespace