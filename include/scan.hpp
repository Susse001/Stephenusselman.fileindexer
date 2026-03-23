#pragma once

#include <vector>
#include <filesystem>
#include "file_record.hpp"

/**
 * @brief Recursively scans a directory and collects FileRecord entries.
 *
 * Traverses the directory tree starting at the specified root path and
 * appends discovered files and directories to the records collection.
 *
 * @param root Root directory to scan
 * @param records Output collection of FileRecord entries
 */
void scan_directory(
    const std::filesystem::path& root,
    std::vector<FileRecord>& records);
