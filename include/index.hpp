#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

#include "file_record.hpp"

/**
 * @brief In-memory index of FileRecord entries with lookup helpers.
 *
 * FileIndex stores file records and builds auxiliary lookup structures
 * to support fast searches by extension, filename, and size.
 */
class FileIndex {
public:
    /**
     * @brief Constructs a FileIndex from a collection of FileRecord entries.
     *
     * @param records File records used to build the index
     */
    explicit FileIndex(std::vector<FileRecord> records);

    /**
     * @brief Returns all records stored in the index.
     *
     * @return Reference to internal record collection
     */
    const std::vector<FileRecord>& records() const { return records_; }

    /**
     * @brief Finds records matching a file extension.
     *
     * @param ext File extension (e.g., ".txt")
     * @return Matching file records
     */
    std::vector<const FileRecord*> find_by_extension(const std::string& ext) const;

    /**
     * @brief Finds records with an exact filename match.
     *
     * @param name Exact filename
     * @return Matching file records
     */
    std::vector<const FileRecord*> find_by_exact_name(const std::wstring& name) const;

    /**
     * @brief Finds records whose filename contains a substring.
     *
     * @param substring Filename substring
     * @return Matching file records
     */
    std::vector<const FileRecord*> find_by_name(const std::string& substring) const;

    /**
     * @brief Finds records within a file size range.
     *
     * @param min Minimum file size
     * @param max Maximum file size
     * @return Matching file records
     */
    std::vector<const FileRecord*> find_by_size(uintmax_t min, uintmax_t max) const;

private:
    /** Stored file records */
    std::vector<FileRecord> records_;

    /** Extension to records lookup */
    std::unordered_map<std::string, std::vector<const FileRecord*>> by_extension_;

    /** Filename to records lookup */
    std::unordered_map<std::wstring, std::vector<const FileRecord*>> by_filename_;
};
