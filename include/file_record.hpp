#pragma once

#include <filesystem>
#include <cstdint>
#include <iosfwd>

namespace {
    constexpr uint32_t FILE_RECORD_VERSION = 1;
}

struct FileRecord {
    std::filesystem::path path;
    uintmax_t size;
    int64_t last_write_time;
    bool is_directory;

    FileRecord(std::filesystem::path p,
               uintmax_t s,
               int64_t lwt,
               bool is_dir)
        : path(std::move(p)),
          size(s),
          last_write_time(lwt),
          is_directory(is_dir)
    {}

    void write(std::ostream& out) const;
    static FileRecord read(std::istream& in);
};
