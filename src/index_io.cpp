#include "index_io.hpp"
#include <fstream>
#include <stdexcept>

static constexpr uint32_t INDEX_VERSION = 1;
static constexpr char MAGIC[4] = { 'F', 'I', 'D', 'X' };

void save_index(const FileIndex& index, const std::string& file)
{
    std::ofstream out(file, std::ios::binary);
    if (!out) throw std::runtime_error("Failed to open index for writing");

    out.write(MAGIC, 4);
    out.write(reinterpret_cast<const char*>(&INDEX_VERSION),
              sizeof(INDEX_VERSION));

    const auto& records = index.records(); // add const accessor
    uint64_t count = records.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& r : records) {
        r.write(out);
    }
}

FileIndex load_index(const std::string& file)
{
    std::ifstream in(file, std::ios::binary);
    if (!in) throw std::runtime_error("Failed to open index for reading");

    char magic[4];
    in.read(magic, 4);
    if (std::memcmp(magic, MAGIC, 4) != 0) {
        throw std::runtime_error("Invalid index file");
    }

    uint32_t version;
    in.read(reinterpret_cast<char*>(&version), sizeof(version));
    if (version != INDEX_VERSION) {
        throw std::runtime_error("Unsupported index version");
    }

    uint64_t count;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    std::vector<FileRecord> records;
    records.reserve(count);

    for (uint64_t i = 0; i < count; ++i) {
        records.push_back(FileRecord::read(in));
    }

    return FileIndex(std::move(records));
}
