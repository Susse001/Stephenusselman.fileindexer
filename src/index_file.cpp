#include "index_file.hpp"
#include <fstream>
#include <stdexcept>
#include <cstring>

namespace {
    constexpr char INDEX_MAGIC[8] = {'F','I','D','X','v','1','\0','\0'};
    constexpr uint32_t INDEX_VERSION = 1;
}

void write_index_file(
    const std::filesystem::path& path,
    const std::vector<FileRecord>& records
)
{
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open index file for writing");
    }

    // Header
    out.write(INDEX_MAGIC, sizeof(INDEX_MAGIC));
    out.write(reinterpret_cast<const char*>(&INDEX_VERSION), sizeof(INDEX_VERSION));

    const uint64_t count = records.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    // Records
    for (const auto& record : records) {
        record.write(out);
    }
}

std::vector<FileRecord> read_index_file(
    const std::filesystem::path& path
)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open index file for reading");
    }

    // Validate
    char magic[8];
    in.read(magic, sizeof(magic));
    if (std::memcmp(magic, INDEX_MAGIC, sizeof(magic)) != 0) {
        throw std::runtime_error("Invalid index file format");
    }

    // Validate version
    uint32_t version = 0;
    in.read(reinterpret_cast<char*>(&version), sizeof(version));
    if (version != INDEX_VERSION) {
        throw std::runtime_error("Unsupported index file version");
    }

    // Read count
    uint64_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    std::vector<FileRecord> records;
    records.reserve(static_cast<size_t>(count));

    for (uint64_t i = 0; i < count; ++i) {
        records.push_back(FileRecord::read(in));
    }

    return records;
}
