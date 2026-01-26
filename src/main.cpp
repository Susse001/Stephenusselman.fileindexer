#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <index.hpp>

namespace fs = std::filesystem;

static void scan_directory(
    const fs::path& root,
    std::vector<FileRecord>& records)
{
    std::error_code ec;

    for (fs::recursive_directory_iterator it(root, fs::directory_options::skip_permission_denied, ec);
         it != fs::recursive_directory_iterator();
         it.increment(ec))
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

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: file_indexer <directory>\n";
        return 1;
    }

    fs::path root{argv[1]};

    if (!fs::exists(root))
    {
        std::cerr << "Path does not exist\n";
        return 1;
    }

    std::vector<FileRecord> records;
    scan_directory(root, records);

    std::cout << "Indexed files: " << records.size() << "\n";

    FileIndex index{std::move(records)};

    auto exe_files = index.find_by_extension(".exe");
    std::cout << "EXE files: " << exe_files.size() << '\n';

    auto file_contains = index.find_by_name("log");
    std::cout << "Files containing 'log': " << file_contains.size() << '\n';

    auto large = index.find_by_size(100 * 1024 * 1024, UINTMAX_MAX);
    std::cout << "Files >100MB: " << large.size() << '\n';

    return 0;
}

