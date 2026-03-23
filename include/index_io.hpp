#pragma once

#include <string>
#include "index.hpp"

/**
 * @brief Saves a FileIndex to disk.
 *
 * Serializes the index and writes it to the specified file.
 *
 * @param index FileIndex to save
 * @param file Output file path
 */
void save_index(const FileIndex& index, const std::string& file);

/**
 * @brief Loads a FileIndex from disk.
 *
 * Reads and deserializes an index from the specified file.
 *
 * @param file Input file path
 * @return Loaded FileIndex
 */
FileIndex load_index(const std::string& file);
