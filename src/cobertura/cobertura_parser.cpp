#include <stdexcept>
#include <string>

#include "spdlog/spdlog.h"

#include "cobertura_parser.hpp"

long double parseNumber(const auto &value)
{
  try {
    return std::stold(value);
  } catch (const std::invalid_argument &) {
    throw std::invalid_argument(std::string("Cannot convert to number: ") + value);
  }
}

void CoberturaParser::on_tag_start(const std::vector<std::string> &tagXmlPath,
  const std::vector<simple_cpp::xml::Attribute> &attributes)
{
  spdlog::debug("start element {}", tagXmlPath.back());
  static const std::vector<std::string> searchPath{ "coverage" };
  static const std::string attr_line_rate("line-rate");
  static const std::string attr_branch_rate("branch-rate");
  if (tagXmlPath == searchPath) {
    for (auto [attribute, value] : attributes) {
      if (attribute == attr_line_rate) {
        coverageInfo.lineCoverage = parseNumber(value);
      } else if (attribute == attr_branch_rate) {
        coverageInfo.branchCoverage = parseNumber(value);
      }
    }
  }
}

CoberturaCoverage CoberturaParser::getCoverageInfo() const
{
  return coverageInfo;
}