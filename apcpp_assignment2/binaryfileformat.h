#pragma once

#include "FileContent.h"

void writeBinary(const FileContent & content, const std::string & filename);
FileContent readBinary(const std::string & filename, bool skipHeader = false);
