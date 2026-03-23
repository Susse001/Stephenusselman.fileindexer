#pragma once

#include <string>
#include <cstdint>
#include <filesystem>
#include <optional>

/**
 * @brief Command-line options controlling query and filtering behavior.
 *
 * This structure is populated from CLI arguments and passed to the
 * query system to control filtering, matching, and result limits.
 */
struct Options {
    /** File extension filter (e.g., ".exe") */
    std::string extension;

    /** Exact filename match (wide string for filesystem compatibility) */
    std::wstring exact_name;

    /** Filename substring match */
    std::string name_substring;

    /** Optional regex pattern for filename matching */
    std::optional<std::string> name_regex;

    /** Minimum file size in bytes */
    uintmax_t min_size = 0;

    /** Maximum file size in bytes */
    uintmax_t max_size = UINTMAX_MAX;

    /** Maximum number of results to return */
    size_t limit = 50;

    /** Path to the index file */
    std::filesystem::path index_path;

    /** Include files modified after this timestamp */
    std::optional<int64_t> modified_after;

    /** Include files modified before this timestamp */
    std::optional<int64_t> modified_before;
};

/**
 * @brief Parses command-line arguments into an Options structure.
 *
 * @param argc Argument count from main
 * @param argv Argument vector from main
 * @return Populated Options structure
 */
Options parse_args(int argc, char* argv[]);

/**
 * @brief Prints command-line usage information.
 */
void print_help();
