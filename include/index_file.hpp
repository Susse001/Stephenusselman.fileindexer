#pragma once

#include <vector>
#include <filesystem>
#include "file_record.hpp"

/**
 * @brief Writes a collection of FileRecord entries to an index file.
 *
 * The index file is written in a binary format and may overwrite
 * any existing file at the specified path.
 *
 * @param path Path to the index file
 * @param records Collection of file records to write
 */
void write_index_file(
    const std::filesystem::path& path,
    const std::vector<FileRecord>& records
);

/**
 * @brief Reads FileRecord entries from an index file.
 *
 * Reads the binary index file and reconstructs the stored
 * FileRecord collection.
 *
 * @param path Path to the index file
 * @return Vector of FileRecord entries read from the file
 */
std::vector<FileRecord> read_index_file(
    const std::filesystem::path& path
);
