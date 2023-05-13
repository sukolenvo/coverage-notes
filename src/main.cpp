#include <string>

#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "spdlog/spdlog.h"
#include <fmt/core.h>

#include "jacoco/JacocoParser.h"

template<> struct fmt::formatter<CoverageInfo> : fmt::formatter<std::string>
{
  auto format(CoverageInfo coverageInfo, format_context &ctx) -> decltype(ctx.out())
  {
    return format_to(ctx.out(), "{}", coverageInfo.print());
  }
};

int main(int argc, const char **argv)
{
  CLI::App app{ "App description" };

  std::string filename = "";
  app.add_option("-j,--jacoco", filename, "Jacoco XML report")->required();

  CLI11_PARSE(app, argc, argv);
  try {
    JacocoParser parser;
    parser.set_throw_messages(true);
    parser.parse_file(filename);
    spdlog::info("Parsed coverage: \n{}", parser.getCoverageInfo());
  } catch (const xmlpp::exception &ex) {
    spdlog::error("Failed to parse xml {}", ex.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
