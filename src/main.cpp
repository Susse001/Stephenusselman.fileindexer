#include <iostream>
#include <filesystem>
#include "cli.hpp"
#include "scan.hpp"
#include "query.hpp"
#include "index.hpp"
#include "index_file.hpp"

/**
 * @file main.cpp
 * @brief Entry point for the file indexing and query tool.
 *
 * Program flow:
 * 1. Determine root directory and index file location
 * 2. Load existing index if present, otherwise scan directory and create index
 * 3. Build FileIndex for fast queries
 * 4. Parse command-line options
 * 5. Execute query
 * 6. Print results (limited by user options)
 */
int main(int argc, char* argv[])
{
    // Require at least root directory argument
    if (argc < 2) {
        print_help();
        return 1;
    }

    std::filesystem::path root = argv[1];
    std::filesystem::path index_path = root / ".file_index.bin";

    std::vector<FileRecord> records;

    // Load existing index if available, otherwise scan directory
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

    // Build in-memory index for fast lookups
    FileIndex index{std::move(records)};

    // Parse command-line query options
    Options opts = parse_args(argc, argv);

    // Execute query
    auto results = run_query(index, opts);

    // Print results up to limit
    size_t count = 0;
    for (const auto* r : results) {
        if (count++ >= opts.limit) break;
        std::cout << r->path.string() << '\n';
    }

    // Print summary
    std::cout << "Results shown: "
              << std::min(results.size(), opts.limit)
              << " / " << results.size() << '\n';

    return 0;
}
