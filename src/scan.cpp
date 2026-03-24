#include "scan.hpp"
#include "file_record.hpp"
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;

/**
 * Recursively scans a directory tree and collects FileRecord entries.
 *
 * Uses recursive_directory_iterator and skips directories that cannot be
 * accessed due to permission errors. Errors are handled using std::error_code
 * to avoid exceptions during traversal.
 */
void scan_directory(const fs::path& root, std::vector<FileRecord>& records)
{
    std::error_code ec;

    for (fs::recursive_directory_iterator it(
             root, fs::directory_options::skip_permission_denied, ec);
         it != fs::recursive_directory_iterator();
         it.increment(ec))
    {
        // Skip entries that produced an error
        if (ec) {
            ec.clear();
            continue;
        }

        const fs::directory_entry& entry = *it;

        // Determine file type and size
        const bool is_dir = entry.is_directory(ec);
        const uintmax_t size =
            (!is_dir && entry.is_regular_file(ec)) ? entry.file_size(ec) : 0;

        // Get last write time
        const int64_t lwt =
            entry.last_write_time(ec).time_since_epoch().count();

        // Add record to collection
        records.emplace_back(
            entry.path(),
            size,
            lwt,
            is_dir
        );
    }
}
