#include <stdexcept>
#include <string>

#include "spdlog/spdlog.h"

#include "JacocoParser.h"

long parseNumber(const auto &value)
{
  try {
    return std::stoi(value);
  } catch (const std::invalid_argument &e) {
    throw std::invalid_argument("Cannot convert to integer value: ");
  }
}

void JacocoParser::on_tag_start(const std::vector<std::string> &tagXmlPath, const std::vector<simple_cpp::xml::Attribute> &attributes)
{
  spdlog::debug("start element {}", tagXmlPath.back());
  static const std::vector<std::string> searchPath{ "report", "counter" };
  static const std::string attr_type("type");
  static const std::string attr_missed("missed");
  static const std::string attr_covered("covered");
  if (tagXmlPath == searchPath) {
    Counter counter;
    for (auto [attribute, value] : attributes) {
      if (attribute == attr_type) {
        counter.type = value;
      } else if (attribute == attr_missed) {
        const long missed = parseNumber(value);
        if (missed < 0) {
          throw std::invalid_argument(std::string("Missed attribute cannot be negative, got: ") + value);
        }
        counter.missed = static_cast<unsigned long>(missed);
      } else if (attribute == attr_covered) {
        const long covered = parseNumber(value);
        if (covered < 0) {
          throw std::invalid_argument(std::string("Covered attribute cannot be negative, got: ") + value);
        }
        counter.covered = static_cast<unsigned long>(covered);
      }
    }
    saveCounter(counter);
  }
}

void JacocoParser::saveCounter(const Counter &counter)
{
  static const std::string type_instruction("INSTRUCTION");
  static const std::string type_branch("BRANCH");
  static const std::string type_line("LINE");
  static const std::string type_complexity("COMPLEXITY");
  static const std::string type_method("METHOD");
  static const std::string type_class("CLASS");
  if (counter.type.empty()) {
    spdlog::warn("Ignoring counter without type");
    return;
  }
  if (counter.type == type_instruction) {
    coverageInfo.setInstructionMissed(counter.missed);
    coverageInfo.setInstructionCovered(counter.covered);
  } else if (counter.type == type_branch) {
    coverageInfo.setBranchMissed(counter.missed);
    coverageInfo.setBranchCovered(counter.covered);
  } else if (counter.type == type_line) {
    coverageInfo.setLineMissed(counter.missed);
    coverageInfo.setLineCovered(counter.covered);
  } else if (counter.type == type_class) {
    coverageInfo.setClassMissed(counter.missed);
    coverageInfo.setClassCovered(counter.covered);
  } else if (counter.type == type_method) {
    coverageInfo.setMethodMissed(counter.missed);
    coverageInfo.setMethodCovered(counter.covered);
  } else if (counter.type == type_complexity) {
    coverageInfo.setComplexityMissed(counter.missed);
    coverageInfo.setComplexityCovered(counter.covered);
  } else {
    spdlog::warn("Ignoring unknown counter {}", counter.type);
  }
}

CoverageInfo JacocoParser::getCoverageInfo() const
{
  return coverageInfo;
}
