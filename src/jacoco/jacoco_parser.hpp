#ifndef COVERAGE_NOTES_JACOCO_PARSER_HPP
#define COVERAGE_NOTES_JACOCO_PARSER_HPP

#include <vector>

#include "simple_cpp_xml/parser.hpp"

#include "jacoco_coverage.hpp"

class JacocoParser : public simple_cpp::xml::Parser
{
private:
  JacocoCoverage coverageInfo;
  struct Counter
  {
    std::string type{};
    unsigned long missed{};
    unsigned long covered{};
  };
  void saveCounter(const Counter &counter);

public:
  JacocoParser() = default;
  ~JacocoParser() override = default;
  [[nodiscard]] JacocoCoverage getCoverageInfo() const;

protected:
  void on_tag_start(const std::vector<std::string> &tagXmlPath,
    const std::vector<simple_cpp::xml::Attribute> &attributes) override;
};


#endif // COVERAGE_NOTES_JACOCO_PARSER_HPP
