#pragma once

#include "FileContent.h"

void writeText(const FileContent & content, const std::string & filename);
FileContent readText(const std::string & filename, bool skipHeader = false);
