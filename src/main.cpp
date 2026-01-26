#include <iostream>
#include <filesystem>
#include "cli.hpp"
#include "scan.hpp"
#include "query.hpp"
#include "index.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        print_help();
        return 1;
    }

    std::filesystem::path root = argv[1];

    // Scan filesystem
    std::vector<FileRecord> records;
    scan_directory(root, records);
    std::cout << "Indexed files: " << records.size() << '\n';

    // Build FileIndex
    FileIndex index{std::move(records)};

    // Parse CLI options
    Options opts = parse_args(argc, argv);

    // Apply filters
    auto results = run_query(index, opts);

    // Print results (limited output)
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
