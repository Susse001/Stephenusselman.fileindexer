#pragma once

#include <string>
#include "index.hpp"

void save_index(const FileIndex& index, const std::string& file);
FileIndex load_index(const std::string& file);
