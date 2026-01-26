#pragma once

#include <filesystem>
#include <cstdint>

namespace fs = std::filesystem;

struct FileRecord
{
    fs::path path;
    uintmax_t size;
    bool is_directory;
};
