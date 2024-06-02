#pragma once

#include "elfLoader.hpp"
#include <optional>

FileType getFileType(const std::string &path);
std::optional<std::unique_ptr<BaseLoader>> loaderFactory(const std::string &path);