#include "file_record.hpp"
#include <ostream>
#include <istream>
#include <string>
#include <stdexcept>


void FileRecord::write(std::ostream& out) const
{
    const uint32_t version = FILE_RECORD_VERSION;
    out.write(reinterpret_cast<const char*>(&version), sizeof(version));

    const auto u8 = path.u8string();
    const std::string path_str(
        reinterpret_cast<const char*>(u8.data()),
        u8.size()
    );
    const uint32_t path_len = static_cast<uint32_t>(path_str.size());

    out.write(reinterpret_cast<const char*>(&path_len), sizeof(path_len));
    out.write(path_str.data(), path_len);

    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    out.write(reinterpret_cast<const char*>(&last_write_time), sizeof(last_write_time));

    const uint8_t dir_flag = is_directory ? 1 : 0;
    out.write(reinterpret_cast<const char*>(&dir_flag), sizeof(dir_flag));
}

FileRecord FileRecord::read(std::istream& in)
{
    uint32_t version = 0;
    in.read(reinterpret_cast<char*>(&version), sizeof(version));

    if (version != FILE_RECORD_VERSION) {
        throw std::runtime_error("Unsupported FileRecord version");
    }

    uint32_t path_len = 0;
    in.read(reinterpret_cast<char*>(&path_len), sizeof(path_len));

    std::string path_str(path_len, '\0');
    in.read(path_str.data(), path_len);

    uint64_t size = 0;
    int64_t last_write_time = 0;
    uint8_t dir_flag = 0;

    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    in.read(reinterpret_cast<char*>(&last_write_time), sizeof(last_write_time));
    in.read(reinterpret_cast<char*>(&dir_flag), sizeof(dir_flag));

    return FileRecord{
        std::filesystem::path(std::move(path_str)),
        size,
        last_write_time,
        dir_flag != 0
    };
}
