#include "cli.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

Options parse_args(int argc, char* argv[])
{
    Options opts;
    opts.min_size = 0;
    opts.max_size = UINTMAX_MAX;
    opts.limit = 50;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--ext" && i + 1 < argc) {
            opts.extension = argv[++i];
        } else if (arg == "--name" && i + 1 < argc) {
            opts.name_substring = argv[++i];
        } else if (arg == "--min-size" && i + 1 < argc) {
            opts.min_size = std::stoull(argv[++i]);
        } else if (arg == "--max-size" && i + 1 < argc) {
            opts.max_size = std::stoull(argv[++i]);
        } else if (arg == "--limit" && i + 1 < argc) {
            opts.limit = std::stoull(argv[++i]);
        } else if (arg == "--help") {
            print_help();
            std::exit(0);
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            print_help();
            std::exit(1);
        }
    }

    return opts;
}

void print_help()
{
    std::cout << "Usage: file_indexer <root> [options]\n\n"
              << "Options:\n"
              << "  --ext <extension>      Filter by file extension (e.g., .exe)\n"
              << "  --name <substring>     Filter by file name substring (case-insensitive)\n"
              << "  --min-size <bytes>     Filter by minimum file size\n"
              << "  --max-size <bytes>     Filter by maximum file size\n"
              << "  --limit <N>            Limit the number of results shown (default 50)\n"
              << "  --help                 Show this help message\n";
}
