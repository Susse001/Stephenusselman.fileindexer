#include "query.hpp"
#include <algorithm>
#include <cwctype>
#include <string>
#include <regex>
#include <stdexcept>

/**
 * Runs a query against the FileIndex using the provided Options.
 *
 * Query strategy:
 * 1. Use most selective indexed lookup if available
 *    - Exact filename
 *    - Extension
 * 2. Otherwise perform size-based scan
 * 3. Apply additional filters:
 *    - Size range
 *    - Modification time
 *    - Substring match
 *    - Regex match
 * 4. Sort results for deterministic output
 */
std::vector<const FileRecord*> run_query(
    const FileIndex& index,
    const Options& opts)
{
    std::vector<const FileRecord*> results;

    // 1. Use most selective indexed lookup
    if (!opts.exact_name.empty()) {
        results = index.find_by_exact_name(opts.exact_name);
    }
    else if (!opts.extension.empty()) {
        results = index.find_by_extension(opts.extension);
    }
    else {
        // Fallback: size scan
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

    // Apply modification time filters
    if (opts.modified_after || opts.modified_before) {
        results.erase(
            std::remove_if(results.begin(), results.end(),
                [&](const FileRecord* r) {
                    if (r->is_directory) {
                        return true;
                    }

                    if (opts.modified_after &&
                        r->last_write_time <= *opts.modified_after) {
                        return true;
                    }

                    if (opts.modified_before &&
                        r->last_write_time >= *opts.modified_before) {
                        return true;
                    }

                    return false;
                }),
            results.end());
    }

    // Apply case-insensitive substring filter
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

    // Apply regex filter
    if (opts.name_regex) {
        std::regex rx;

        try {
            rx = std::regex(*opts.name_regex,
                            std::regex::ECMAScript | std::regex::icase);
        } catch (const std::regex_error&) {
            throw std::runtime_error(
                "Invalid regex pattern: " + *opts.name_regex);
        }

        results.erase(
            std::remove_if(results.begin(), results.end(),
                [&](const FileRecord* r) {
                    const std::wstring name = r->path.filename().native();
                    const std::string narrow(name.begin(), name.end());
                    return !std::regex_search(narrow, rx);
                }),
            results.end());
    }

    // Sort results by path for deterministic output
    std::sort(results.begin(), results.end(),
        [](const FileRecord* a, const FileRecord* b) {
            return a->path < b->path;
        });

    return results;
}
