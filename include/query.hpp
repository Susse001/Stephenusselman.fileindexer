#pragma once

#include <vector>
#include "file_record.hpp"
#include "index.hpp"
#include "cli.hpp"

/**
 * @brief Applies query filters to a FileIndex and returns matching results.
 *
 * Filters are applied based on the provided Options structure and
 * the resulting records are returned in sorted order.
 *
 * @param index FileIndex to query
 * @param opts Query and filter options
 * @return Filtered and sorted file records
 */
std::vector<const FileRecord*> run_query(
    const FileIndex& index,
    const Options& opts);
