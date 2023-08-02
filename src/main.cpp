#include <iostream>
#include <string>

#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include <simple_cpp_xml/exception.hpp>
#include <spdlog/spdlog.h>

#include "jacoco/JacocoParser.h"

auto readFile(auto path)
{
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::stringstream buffer;
    buffer << stream.rdbuf();
    stream.close();
    return buffer.str();
  } else {
    throw std::runtime_error("Failed to open task input file");
  }
}

int main(int argc, const char **argv)
{
  CLI::App app{ "App description" };

  std::string filename = "";
  app.add_option("-j,--jacoco", filename, "Jacoco XML report")->required();

  CLI11_PARSE(app, argc, argv);

  const auto content = readFile(filename);
  try {
    JacocoParser parser;
    parser.parse(content);
    std::cout << parser.getCoverageInfo().print();
  } catch (const simple_cpp::xml::ParseError &ex) {
    spdlog::error("Failed to parse xml {}", ex.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
