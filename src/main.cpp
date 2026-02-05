#include <iostream>
#include <filesystem>
#include "cli.hpp"
#include "scan.hpp"
#include "query.hpp"
#include "index.hpp"
#include "index_file.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        print_help();
        return 1;
    }

    std::filesystem::path root = argv[1];
    std::filesystem::path index_path = root / ".file_index.bin";

    std::vector<FileRecord> records;

    if (std::filesystem::exists(index_path)) {
        records = read_index_file(index_path);
        std::cout << "Loaded index from "
                  << std::filesystem::absolute(index_path)
                  << " (" << records.size() << " entries)\n";
    } else {
        scan_directory(root, records);
        std::cout << "Saved index to "
                  << std::filesystem::absolute(index_path)
                  << " (" << records.size() << " entries)\n";

        write_index_file(index_path, records);
    }

    FileIndex index{std::move(records)};

    Options opts = parse_args(argc, argv);

    auto results = run_query(index, opts);

    size_t count = 0;
    for (const auto* r : results) {
        if (count++ >= opts.limit) break;
        std::cout << r->path.string() << '\n';
    }

    std::cout << "Results shown: "
              << std::min(results.size(), opts.limit)
              << " / " << results.size() << '\n';

    return 0;
}
