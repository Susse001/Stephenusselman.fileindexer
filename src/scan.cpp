#include "scan.hpp"
#include "file_record.hpp"
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;

void scan_directory(const fs::path& root, std::vector<FileRecord>& records)
{
    std::error_code ec;

    for (fs::recursive_directory_iterator it(
             root, fs::directory_options::skip_permission_denied, ec);
         it != fs::recursive_directory_iterator();
         it.increment(ec))
    {
        if (ec) {
            ec.clear();
            continue;
        }

        const fs::directory_entry& entry = *it;

        const bool is_dir = entry.is_directory(ec);
        const uintmax_t size =
            (!is_dir && entry.is_regular_file(ec)) ? entry.file_size(ec) : 0;

        const int64_t lwt =
            entry.last_write_time(ec).time_since_epoch().count();

        records.emplace_back(
            entry.path(),
            size,
            lwt,
            is_dir
        );
    }
}
