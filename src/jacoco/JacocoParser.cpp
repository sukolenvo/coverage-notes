#include <string>

#include "spdlog/spdlog.h"

#include "JacocoParser.h"

void JacocoParser::on_start_element(const xmlpp::ustring &name, const xmlpp::SaxParser::AttributeList &attributeList)
{
  spdlog::debug("start element {}", name);
  static const std::vector<xmlpp::ustring> searchPath{ "report" };
  static const xmlpp::ustring attr_type = "type";
  static const xmlpp::ustring attr_missed = "missed";
  static const xmlpp::ustring attr_covered = "covered";
  if (name == "counter" && xmlPath == searchPath) {
    Counter counter;
    for (const auto &[attribute, value] : attributeList) {
      if (attribute == attr_type) {
        counter.type = value;
      } else if (attribute == attr_missed) {
        const long missed = std::stoi(value);
        if (missed < 0) {
          spdlog::critical("Missed attribute cannot be negative, got {}", missed);
        }
        counter.missed = static_cast<unsigned long>(missed);
      } else if (attribute == attr_covered) {
        const long covered = std::stoi(value);
        if (covered < 0) {
          spdlog::critical("Covered attribute cannot be negative, got {}", covered);
        }
        counter.covered = static_cast<unsigned long>(covered);
      }
    }
    saveCounter(counter);
  }
  xmlPath.push_back(name);
}

void JacocoParser::saveCounter(const Counter &counter)
{
  static const xmlpp::ustring type_instruction = "INSTRUCTION";
  static const xmlpp::ustring type_branch = "BRANCH";
  static const xmlpp::ustring type_line = "LINE";
  static const xmlpp::ustring type_complexity = "COMPLEXITY";
  static const xmlpp::ustring type_method = "METHOD";
  static const xmlpp::ustring type_class = "CLASS";
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

void JacocoParser::on_end_element(const xmlpp::ustring &name)
{
  spdlog::debug("end element {}", name);
  if (xmlPath.empty() || xmlPath.back() != name) {
    std::string xmlPathStr = "/";
    for (const auto &element : xmlPath) {
      xmlPathStr.append(element);
      xmlPathStr.append("/");
    }
    spdlog::critical(
      "Invalid XML: open tags don't match closed tags. XmlPath: {}. But got close tag {}", xmlPathStr, name);
  }
  xmlPath.pop_back();
}

void JacocoParser::on_warning(const xmlpp::ustring &text)
{
  spdlog::warn("Warning: {}", text);
}

void JacocoParser::on_error(const xmlpp::ustring &text)
{
  spdlog::error("Error: {}", text);
}

void JacocoParser::on_fatal_error(const xmlpp::ustring &text)
{
  spdlog::critical("Fatal error: {}", text);
}

CoverageInfo JacocoParser::getCoverageInfo() const
{
  return coverageInfo;
}
