#include "cli.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <filesystem>

Options parse_args(int argc, char* argv[])
{
    Options opts;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--ext" && i + 1 < argc) {
            opts.extension = argv[++i];
        }
        else if (arg == "--exact-name" && i + 1 < argc) {
            opts.exact_name = std::filesystem::path(argv[++i]).native();
        }
        else if (arg == "--name" && i + 1 < argc) {
            opts.name_substring = argv[++i];
        }
        else if (arg == "--min-size" && i + 1 < argc) {
            opts.min_size = std::stoull(argv[++i]);
        }
        else if (arg == "--max-size" && i + 1 < argc) {
            opts.max_size = std::stoull(argv[++i]);
        }
        else if (arg == "--limit" && i + 1 < argc) {
            opts.limit = std::stoull(argv[++i]);
        }
        else if (arg == "--index" && i + 1 < argc) {
        opts.index_path = std::filesystem::path(argv[++i]);
        }
        else if (arg == "--help") {
            print_help();
            std::exit(0);
        }
        else {
            std::cerr << "Unknown option: " << arg << "\n";
            print_help();
            std::exit(1);
        }
    }

    return opts;
}

void print_help()
{
    std::cout
        << "Usage: file_indexer <root> [options]\n\n"
        << "Options:\n"
        << "  --exact-name <name>    Match exact filename (most selective)\n"
        << "  --ext <extension>     Filter by file extension (e.g., .exe)\n"
        << "  --name <substring>    Filter by filename substring (case-insensitive)\n"
        << "  --min-size <bytes>    Filter by minimum file size\n"
        << "  --max-size <bytes>    Filter by maximum file size\n"
        << "  --limit <N>           Limit number of results shown (default 50)\n"
        << "  --index <path>        Path to index file for caching results\n"
        << "  --help                Show this help message\n";
}
