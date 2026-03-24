#include "cli.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <filesystem>

/**
 * Parses command-line arguments and populates an Options structure.
 *
 * Expected format:
 *   file_indexer <root> [options]
 *
 * Starts parsing from argv[2] because argv[1] is the root directory.
 * On invalid options, prints an error and exits.
 */
Options parse_args(int argc, char* argv[])
{
    Options opts;

    // Iterate over optional command-line arguments
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        // File extension filter
        if (arg == "--ext" && i + 1 < argc) {
            opts.extension = argv[++i];
        }
        // Exact filename match
        else if (arg == "--exact-name" && i + 1 < argc) {
            opts.exact_name = std::filesystem::path(argv[++i]).native();
        }
        // Filename substring match
        else if (arg == "--name" && i + 1 < argc) {
            opts.name_substring = argv[++i];
        }
        // Minimum file size
        else if (arg == "--min-size" && i + 1 < argc) {
            opts.min_size = std::stoull(argv[++i]);
        }
        // Maximum file size
        else if (arg == "--max-size" && i + 1 < argc) {
            opts.max_size = std::stoull(argv[++i]);
        }
        // Modified after timestamp
        else if (arg == "--modified-after" && i + 1 < argc) {
            opts.modified_after = std::stoll(argv[++i]);
        }
        // Modified before timestamp
        else if (arg == "--modified-before" && i + 1 < argc) {
            opts.modified_before = std::stoll(argv[++i]);
        }
        // Filename regex match
        else if (arg == "--name-regex" && i + 1 < argc) {
            opts.name_regex = argv[++i];
        }
        // Result limit
        else if (arg == "--limit" && i + 1 < argc) {
            opts.limit = std::stoull(argv[++i]);
        }
        // Index file path
        else if (arg == "--index" && i + 1 < argc) {
            opts.index_path = std::filesystem::path(argv[++i]);
        }
        // Help option
        else if (arg == "--help") {
            print_help();
            std::exit(0);
        }
        // Unknown option
        else {
            std::cerr << "Unknown option: " << arg << "\n";
            print_help();
            std::exit(1);
        }
    }

    return opts;
}

/**
 * Prints command-line usage and available options.
 */
void print_help()
{
    std::cout
        << "Usage: file_indexer <root> [options]\n\n"
        << "Options:\n"
        << "  --exact-name <name>    Match exact filename (most selective)\n"
        << "  --ext <extension>     Filter by file extension (e.g., .exe)\n"
        << "  --name <substring>    Filter by filename substring (case-insensitive)\n"
        << "  --name-regex <pattern> Match filename using regex (ECMAScript)\n"
        << "  --min-size <bytes>    Filter by minimum file size\n"
        << "  --max-size <bytes>    Filter by maximum file size\n"
        << "  --modified-after <epoch>   Files modified after this Unix timestamp\n"
        << "  --modified-before <epoch>  Files modified before this Unix timestamp\n"
        << "  --limit <N>           Limit number of results shown (default 50)\n"
        << "  --index <path>        Path to index file for caching results\n"
        << "  --help                Show this help message\n";
}
