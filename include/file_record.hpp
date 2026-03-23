#pragma once

#include <filesystem>
#include <cstdint>
#include <iosfwd>

/**
 * @brief Internal file record format version.
 *
 * Used to version the binary serialization format for FileRecord.
 * Increment this value if the FileRecord serialization layout changes.
 */
namespace {
    constexpr uint32_t FILE_RECORD_VERSION = 1;
}

/**
 * @brief Represents a filesystem entry stored in the index.
 *
 * FileRecord contains metadata for a file or directory including:
 * - Full filesystem path
 * - File size in bytes
 * - Last write timestamp
 * - Directory flag
 *
 * This struct also supports binary serialization and deserialization
 * through the write() and read() methods.
 */
struct FileRecord {
    /** Full path to the file or directory */
    std::filesystem::path path;

    /** File size in bytes (undefined for some directories depending on platform) */
    uintmax_t size;

    /** Last modification time stored as integer timestamp */
    int64_t last_write_time;

    /** True if this record represents a directory */
    bool is_directory;

    /**
     * @brief Constructs a FileRecord with the provided metadata.
     *
     * @param p Path to the file or directory
     * @param s File size in bytes
     * @param lwt Last write time as integer timestamp
     * @param is_dir True if the path represents a directory
     */
    FileRecord(std::filesystem::path p,
               uintmax_t s,
               int64_t lwt,
               bool is_dir)
        : path(std::move(p)),
          size(s),
          last_write_time(lwt),
          is_directory(is_dir)
    {}

    /**
     * @brief Writes this FileRecord to a binary output stream.
     *
     * The record is written in a versioned binary format so that
     * future versions of the program can maintain compatibility.
     *
     * @param out Output stream to write the record to
     */
    void write(std::ostream& out) const;

    /**
     * @brief Reads a FileRecord from a binary input stream.
     *
     * This function reads data written by FileRecord::write() and
     * reconstructs a FileRecord object.
     *
     * @param in Input stream to read from
     * @return FileRecord Deserialized file record
     */
    static FileRecord read(std::istream& in);
};
