#pragma once
#include <vector>
#include "file_record.hpp"
#include "index.hpp"
#include "cli.hpp"

// Applies Options filters to a FileIndex and returns filtered, sorted results.
std::vector<const FileRecord*> run_query(
    const FileIndex& index,
    const Options& opts);

