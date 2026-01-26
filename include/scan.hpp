#pragma once
#include <vector>
#include <filesystem>
#include "file_record.hpp"

// Recursively scans `root` and fills `records`.
// Skips permission-denied directories, sets file sizes for regular files
void scan_directory(
    const std::filesystem::path& root,
    std::vector<FileRecord>& records);

