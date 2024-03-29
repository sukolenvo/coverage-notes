#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <simple_cpp_github_rest/client.hpp>
#include <simple_cpp_github_rest/create_commit.hpp>
#include <simple_cpp_github_rest/create_reference.hpp>
#include <simple_cpp_github_rest/create_tree.hpp>
#include <simple_cpp_github_rest/exception.hpp>
#include <simple_cpp_github_rest/get_commit.hpp>
#include <simple_cpp_github_rest/get_pull.hpp>
#include <simple_cpp_github_rest/get_reference.hpp>
#include <simple_cpp_github_rest/get_repository_content.hpp>
#include <simple_cpp_github_rest/list_pulls_by_commit.hpp>
#include <simple_cpp_github_rest/update_pull.hpp>
#include <simple_cpp_github_rest/update_reference.hpp>
#include <simple_cpp_xml/exception.hpp>
#include <spdlog/spdlog.h>

#include "cobertura/cobertura_parser.hpp"
#include "jacoco/jacoco_parser.hpp"

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

std::string read_env(const std::string &name)
{
  const char *value = std::getenv(name.c_str());
  if (value == nullptr) {
    return "";
  }
  return value;
}

template<> struct fmt::formatter<CoverageSummary> : fmt::formatter<std::string>
{
  auto format(CoverageSummary coverageSummary, format_context &ctx) const -> decltype(ctx.out())
  {
    return format_to(ctx.out(), "{}", coverageSummary.print());
  }
};

std::optional<std::string> read_base_tree(simple_cpp::github_rest::Client &client, const std::string &notesRef)
{
  simple_cpp::github_rest::GetReferenceRequest getReferenceRequest{ notesRef };
  std::string commitSha{};
  try {
    const auto reference = getReferenceRequest.execute(client);
    if (reference.object.type != "commit") {
      throw std::runtime_error(std::string("Expected reference to point to commit, but got :") + reference.object.type);
    }
    commitSha = reference.object.sha;
  } catch (const simple_cpp::github_rest::GithubRestException &) {
    // TODO check response code is 404
  }
  if (commitSha.empty()) {
    return {};
  }
  simple_cpp::github_rest::GetCommitRequest getCommitRequest{ commitSha };
  const auto commit = getCommitRequest.execute(client);
  return { commit.tree.sha };
}

void create_note(simple_cpp::github_rest::Client &client,
  const CoverageSummary &coverage,
  const std::string &ref,
  const std::string &commitSha)
{
  const auto base_tree = read_base_tree(client, ref);
  const simple_cpp::github_rest::CreateTreeRequestBody createTreeRequestBody{
    .tree = { { .path = commitSha, .mode = simple_cpp::github_rest::MODE_FILE, .content = coverage.print() } },
    .base_tree = base_tree
  };
  simple_cpp::github_rest::CreateTreeRequest createTreeRequest{ createTreeRequestBody };
  const auto tree = createTreeRequest.execute(client);
  const simple_cpp::github_rest::CreateCommitRequestBody createCommitRequestBody{ .message = "save coverage details",
    .author = { .name = "Coverage notes app", .email = "sukolenvo+cn@gmail.com" },
    .tree = tree.sha };
  simple_cpp::github_rest::CreateCommitRequest createCommitRequest{ createCommitRequestBody };
  const auto commit = createCommitRequest.execute(client);
  if (base_tree.has_value()) {
    const simple_cpp::github_rest::UpdateReferenceRequestBody updateReferenceRequestBody{ .sha = commit.sha };
    simple_cpp::github_rest::UpdateReferenceRequest updateReferenceRequest{ ref, updateReferenceRequestBody };
    updateReferenceRequest.execute(client);
  } else {
    const simple_cpp::github_rest::CreateReferenceRequestBody createReferenceRequestBody{ .ref = "refs/" + ref,
      .sha = commit.sha };
    simple_cpp::github_rest::CreateReferenceRequest createReferenceRequest{ createReferenceRequestBody };
    createReferenceRequest.execute(client);
  }
}

CoverageSummary
  get_coverage_info(simple_cpp::github_rest::Client &client, const std::string &commitSha, const std::string &notesRef)
{
  simple_cpp::github_rest::GetRepositoryContentRequest getRepositoryContentRequest{ commitSha, notesRef };
  try {
    const auto content = getRepositoryContentRequest.execute(client);
    return CoverageSummary(content.parse_content());
  } catch (const simple_cpp::github_rest::GithubRestApiException &) {
    return {};
  }
}

void update_pull(simple_cpp::github_rest::Client &client,
  const simple_cpp::github_rest::Pull &pull,
  const std::string &notesRef)
{
  const auto baseCoverage = get_coverage_info(client, pull.base.sha, notesRef);
  spdlog::info("Base coverage: {}", baseCoverage);
  const auto headCoverage = get_coverage_info(client, pull.head.sha, notesRef);
  spdlog::info("Head coverage: {}", baseCoverage);
  if (headCoverage.empty()) {
    spdlog::warn("Can't update PR {}. Coverage info missing", pull.number);
    return;
  }
  const auto diff = (baseCoverage.empty() ? headCoverage : baseCoverage).diff(headCoverage);
  spdlog::info("Updating PR {}. Diff: {}", pull.number, diff);
  std::string body = pull.body.value_or("");
  body.erase(std::remove(body.begin(), body.end(), '\r'), body.cend());
  auto markerPos = body.find(diff.substr(0, 20));
  if (markerPos != std::string::npos) {
    body.erase(markerPos);
  }
  if (!body.empty() && body.back() != '\n') {
    body += '\n';
  }
  body += diff;
  const simple_cpp::github_rest::UpdatePullRequestBody updatePullRequestBody{ .body = body };
  simple_cpp::github_rest::UpdatePullRequest updatePullRequest{ pull.number, updatePullRequestBody };
  updatePullRequest.execute(client);
}

CoverageSummary parseCoverage(const CLI::App &app)
{
  if (!app.get_option("--jacoco")->empty()) {
    std::string filename;
    app.get_option("--jacoco")->results(filename);
    const auto content = readFile(filename);
    JacocoParser parser;
    parser.parse(content);
    return parser.getCoverageInfo().buildSummary();
  }
  if (!app.get_option("--cobertura")->empty()) {
    std::string filename;
    app.get_option("--cobertura")->results(filename);
    const auto content = readFile(filename);
    CoberturaParser parser;
    parser.parse(content);
    return parser.getCoverageInfo().buildSummary();
  }
  spdlog::error("Coverage file required");
  std::exit(EXIT_FAILURE);
}

int main(int argc, const char **argv)
{
  CLI::App app{ "App description" };
  app.require_subcommand();

  std::string filename;
  auto *jacocoOption = app.add_option("-j,--jacoco", filename, "Jacoco XML report");
  auto *coberturaOption = app.add_option("--cobertura", filename, "Cobertura XML report");
  jacocoOption->excludes(coberturaOption)->check(CLI::ExistingFile);
  coberturaOption->excludes(jacocoOption)->check(CLI::ExistingFile);
  std::string repository = read_env("GITHUB_REPOSITORY");
  app.add_option("--github_repository",
    repository,
    "Github repository in format owner/repo_name. Fallback to env variable GITHUB_REPOSITORY");
  std::string token = read_env("GITHUB_TOKEN");
  app.add_option("--github_token", token, "Github token. Fallback to env variable GITHUB_TOKEN");
  std::string notesRef{ "notes/coverage" };
  app.add_option("--notes_ref", notesRef, "Notes reference. Default is notes/coverage");
  auto *createCommitCmd = app.add_subcommand("create-commit", "Create commit note for commit, updates related PRs");
  std::string commit = read_env("GITHUB_SHA");
  createCommitCmd->add_option("--commit_sha", commit, "Commit sha. Fallback to env variable GITHUB_SHA");
  auto *createPrCmd = app.add_subcommand("create-pr", "Add coverage info to PR");
  unsigned long prNumber;
  createPrCmd->add_option("--pr_number", prNumber, "Create PR number")->required(true);
  auto *printCmd =
    app.add_subcommand("print", "Print coverage and exit")->excludes(createCommitCmd)->excludes(createPrCmd);

  CLI11_PARSE(app, argc, argv);

  try {
    if (printCmd->parsed()) {
      const CoverageSummary coverageSummary = parseCoverage(app);
      std::cout << coverageSummary.print();
      return EXIT_SUCCESS;
    }

    if (repository.empty() || repository.find('/') == std::string::npos) {
      spdlog::error("Invalid repository name: {}", repository);
      return EXIT_FAILURE;
    }
    const std::string owner = repository.substr(0, repository.find('/'));
    const std::string repo_name = repository.substr(repository.find('/') + 1);
    if (owner.empty() || repo_name.empty()) {
      spdlog::error("Invalid repository name: {}", repository);
      return EXIT_FAILURE;
    }

    if (token.empty()) {
      spdlog::error("Github token is missing");
      return EXIT_FAILURE;
    }

    simple_cpp::github_rest::Client client{ owner, repo_name, token };

    if (createCommitCmd->parsed()) {
      if (commit.empty()) {
        spdlog::error("Commit sha is missing");
        return EXIT_FAILURE;
      }
      CoverageSummary coverageSummary = parseCoverage(app);
      create_note(client, coverageSummary, notesRef, commit);
      simple_cpp::github_rest::ListPullsByCommitRequest listPullsByCommitRequest{ commit };
      auto pulls = listPullsByCommitRequest.execute(client);
      for (const auto &pull : pulls) {
        update_pull(client, pull, notesRef);
      }
    }

    if (createPrCmd->parsed()) {
      simple_cpp::github_rest::GetPullRequest getPullRequest{ prNumber };
      const auto pull = getPullRequest.execute(client);
      update_pull(client, pull, notesRef);
    }

  } catch (const simple_cpp::github_rest::GithubRestException &ex) {
    spdlog::error("Failed to call github {}", ex.what());
    return EXIT_FAILURE;
  } catch (const simple_cpp::xml::ParseError &ex) {
    spdlog::error("Failed to parse xml {}", ex.what());
    return EXIT_FAILURE;
  } catch (const std::runtime_error &ex) {
    spdlog::error("Error {}", ex.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
