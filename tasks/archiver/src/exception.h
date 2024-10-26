#pragma once

#include <exception>
#include <string>

class ArchiveNameNotPassed : public std::exception {
public:
    const char* what() const noexcept override {
        return "Archive name not passed";
    }
};

class FilenameNotPassed : public std::exception {
public:
    const char* what() const noexcept override {
        return "Filename not passed";
    }
};

class IncorrectArchiveDataFormat : public std::exception {
public:
    const char* what() const noexcept override {
        return "File has incorrect data format";
    }
};

class FileBroken : public std::exception {
public:
    explicit FileBroken(const std::string& filename) : filename_(filename) {
    }

    const char* what() const noexcept override {
        std::string message = "File does not exist: " + filename_;
        const char* c = message.c_str();
        return c;
    }
    
private:
    std::string filename_;
};