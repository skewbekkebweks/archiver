#pragma once

#include <string>

const int FILENAME_END = 256;
const int ONE_MORE_FILE = 257;
const int ARCHIVE_END = 258;

const int BITS_IN_ITEM = 9;

void IncrementBinaryString(std::string& bin_str);