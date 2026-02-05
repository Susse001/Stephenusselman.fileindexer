#pragma once

#include <vector>
#include <filesystem>
#include "file_record.hpp"

void write_index_file(
    const std::filesystem::path& path,
    const std::vector<FileRecord>& records
);

std::vector<FileRecord> read_index_file(
    const std::filesystem::path& path
);
