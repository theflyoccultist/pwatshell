#pragma once

#include <string>

struct FileInfo {
    std::string filename;
    bool isDirectory = false;

    FileInfo(std::string file, bool isDir = false)
        : filename(std::move(file)), isDirectory(isDir) {}
};
