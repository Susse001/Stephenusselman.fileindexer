#pragma once

#include <string>
#include <cstdint>
#include <filesystem>

struct Options {
    std::string extension;
    std::wstring exact_name;
    std::string name_substring;
    uintmax_t min_size = 0;
    uintmax_t max_size = UINTMAX_MAX;
    size_t limit = 50;

    std::filesystem::path index_path;
};

// Parses argv into Options. Exits program on error or --help.
Options parse_args(int argc, char* argv[]);

// Prints usage and CLI contract
void print_help();
