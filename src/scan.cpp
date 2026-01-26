#include "scan.hpp"
#include "file_record.hpp"
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;

void scan_directory(const fs::path& root, std::vector<FileRecord>& records)
{
    std::error_code ec;

    for (fs::recursive_directory_iterator it(root,
             fs::directory_options::skip_permission_denied, ec);
         it != fs::recursive_directory_iterator(); it.increment(ec))
    {
        if (ec) {
            ec.clear();
            continue;
        }

        const fs::directory_entry& entry = *it;

        FileRecord record{
            entry.path(),
            entry.is_regular_file(ec) ? entry.file_size(ec) : 0,
            entry.is_directory(ec)
        };

        records.push_back(record);
    }
}
