#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

#include "file_record.hpp"

namespace fs = std::filesystem;

class FileIndex {
public:
    explicit FileIndex(std::vector<FileRecord> records);

    std::vector<const FileRecord*> find_by_extension(const std::string& ext) const;
    std::vector<const FileRecord*> find_by_name(const std::string& substring) const;
    std::vector<const FileRecord*> find_by_size(uintmax_t min, uintmax_t max) const;

private:
    std::vector<FileRecord> records_;

    std::unordered_map<std::string, std::vector<const FileRecord*>> by_extension_;
};
