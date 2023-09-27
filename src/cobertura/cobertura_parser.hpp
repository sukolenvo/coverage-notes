#ifndef COVERAGE_NOTES_COBERTURA_PARSER_HPP
#define COVERAGE_NOTES_COBERTURA_PARSER_HPP

#include <vector>

#include "simple_cpp_xml/parser.hpp"

#include "cobertura_coverage.hpp"

class CoberturaParser : public simple_cpp::xml::Parser
{
private:
  CoberturaCoverage coverageInfo;

public:
  CoberturaParser() = default;
  ~CoberturaParser() override = default;
  [[nodiscard]] CoberturaCoverage getCoverageInfo() const;

protected:
  void on_tag_start(const std::vector<std::string> &tagXmlPath,
    const std::vector<simple_cpp::xml::Attribute> &attributes) override;
};


#endif // COVERAGE_NOTES_COBERTURA_PARSER_HPP
