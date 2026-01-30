#include "query.hpp"
#include <algorithm>
#include <cwctype>
#include <string>

std::vector<const FileRecord*> run_query(
    const FileIndex& index,
    const Options& opts)
{
    std::vector<const FileRecord*> results;

    // 1. Exact filename (most restrictive)
    if (!opts.exact_name.empty()) {
        results = index.find_by_exact_name(opts.exact_name);
    }
    // 2. Extension
    else if (!opts.extension.empty()) {
        results = index.find_by_extension(opts.extension);
    }
    // 3. Fallback: size scan
    else {
        results = index.find_by_size(opts.min_size, opts.max_size);
    }

    // Apply size filter if not already applied
    if (!(opts.min_size == 0 && opts.max_size == UINTMAX_MAX)) {
        results.erase(
            std::remove_if(results.begin(), results.end(),
                [&](const FileRecord* r) {
                    return r->is_directory ||
                           r->size < opts.min_size ||
                           r->size > opts.max_size;
                }),
            results.end());
    }

    // Apply substring filter last (always expensive)
    if (!opts.name_substring.empty()) {
        std::wstring needle(opts.name_substring.begin(), opts.name_substring.end());
        std::transform(needle.begin(), needle.end(), needle.begin(), ::towlower);

        results.erase(
            std::remove_if(results.begin(), results.end(),
                [&](const FileRecord* r) {
                    std::wstring name = r->path.filename().native();
                    std::transform(name.begin(), name.end(), name.begin(), ::towlower);
                    return name.find(needle) == std::wstring::npos;
                }),
            results.end());
    }

    // Deterministic output
    std::sort(results.begin(), results.end(),
        [](const FileRecord* a, const FileRecord* b) {
            return a->path < b->path;
        });

    return results;
}
