#include "index.hpp"

#include <algorithm>
#include <cwctype>

FileIndex::FileIndex(std::vector<FileRecord> records)
    : records_(std::move(records))
{
    for (const auto& record : records_) {
        if (!record.is_directory) {
            std::string ext = record.path.extension().string();
            by_extension_[ext].push_back(&record);
        }
    }
}

std::vector<const FileRecord*>
FileIndex::find_by_extension(const std::string& ext) const
{
    auto it = by_extension_.find(ext);
    if (it == by_extension_.end()) {
        return {};
    }
    return it->second;
}

std::vector<const FileRecord*>
FileIndex::find_by_name(const std::string& substring) const
{
    std::vector<const FileRecord*> results;

    // Convert needle to lowercase wide string
    std::wstring needle(substring.begin(), substring.end());
    std::transform(needle.begin(), needle.end(), needle.begin(), ::towlower);

    for (const auto& record : records_) {
        std::wstring name = record.path.filename().native();
        std::transform(name.begin(), name.end(), name.begin(), ::towlower);

        if (name.find(needle) != std::wstring::npos) {
            results.push_back(&record);
        }
    }

    return results;
}

std::vector<const FileRecord*>
FileIndex::find_by_size(uintmax_t min, uintmax_t max) const
{
    std::vector<const FileRecord*> results;

    for (const auto& record : records_) {
        if (!record.is_directory && record.size >= min && record.size <= max) {
            results.push_back(&record);
        }
    }
    return results;
}
