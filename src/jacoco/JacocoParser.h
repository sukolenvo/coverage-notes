#ifndef COVERAGE_NOTES_JACOCOPARSER_H
#define COVERAGE_NOTES_JACOCOPARSER_H

#include <vector>

#include "../core/CoverageInfo.h"
#include "libxml++/libxml++.h"

class JacocoParser : public xmlpp::SaxParser
{
private:
  std::vector<xmlpp::ustring> xmlPath;
  CoverageInfo coverageInfo;
  struct Counter {
    xmlpp::ustring type{};
    unsigned long missed{};
    unsigned long covered{};
  };
  void saveCounter(const Counter& counter);
public:
  JacocoParser() = default;
  ~JacocoParser() override = default;
  [[nodiscard]] CoverageInfo getCoverageInfo() const;
protected:
  void on_start_element(const xmlpp::ustring& name, const AttributeList& properties) override;
  void on_end_element(const xmlpp::ustring& name) override;
  void on_warning(const xmlpp::ustring& text) override;
  void on_error(const xmlpp::ustring& text) override;
  void on_fatal_error(const xmlpp::ustring& text) override;
};


#endif // COVERAGE_NOTES_JACOCOPARSER_H
