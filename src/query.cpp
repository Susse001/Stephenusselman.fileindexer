#include "query.hpp"
#include <algorithm>
#include <cwctype>
#include <string>

std::vector<const FileRecord*> run_query(
    const FileIndex& index,
    const Options& opts)
{
    std::vector<const FileRecord*> results =
        index.find_by_size(opts.min_size, opts.max_size);

    // Filter by extension
    if (!opts.extension.empty()) {
        auto ext_results = index.find_by_extension(opts.extension);
        results.erase(
            std::remove_if(results.begin(), results.end(),
                [&](const FileRecord* r) {
                    return std::find(ext_results.begin(), ext_results.end(), r) == ext_results.end();
                }),
            results.end());
    }

    // Filter by name substring, case-insensitive
    if (!opts.name_substring.empty()) {
        std::wstring needle(opts.name_substring.begin(), opts.name_substring.end());
        std::transform(needle.begin(), needle.end(), needle.begin(), ::towlower);

        std::vector<const FileRecord*> filtered;
        for (const auto* r : results) {
            std::wstring name = r->path.filename().native();
            std::transform(name.begin(), name.end(), name.begin(), ::towlower);

            if (name.find(needle) != std::wstring::npos) {
                filtered.push_back(r);
            }
        }

        results = std::move(filtered);
    }

    // Sort by path for deterministic output
    std::sort(results.begin(), results.end(),
        [](const FileRecord* a, const FileRecord* b) {
            return a->path < b->path;
        });

    return results;
}
