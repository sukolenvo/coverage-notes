#ifndef COVERAGE_NOTES_JACOCOPARSER_H
#define COVERAGE_NOTES_JACOCOPARSER_H

#include <vector>

#include "simple_cpp_xml/parser.hpp"

#include "core/CoverageInfo.h"

class JacocoParser : public simple_cpp::xml::Parser
{
private:
  CoverageInfo coverageInfo;
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
  [[nodiscard]] CoverageInfo getCoverageInfo() const;

protected:
  void on_tag_start(const std::vector<std::string> &tagXmlPath,
    const std::vector<simple_cpp::xml::Attribute> &attributes) override;
};


#endif // COVERAGE_NOTES_JACOCOPARSER_H
