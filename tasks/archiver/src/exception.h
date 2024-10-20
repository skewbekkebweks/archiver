#include <exception>
#include <string>

class ArchiveNameNotPassed : public std::exception {
public:
    const char* what() const noexcept override {
        return "Archive name not passed";
    }
};

class FileNameNotPassed : public std::exception {
public:
    const char* what() const noexcept override {
        return "File name not passed";
    }
};

class FileDoesNotExist : public std::exception {
public:
    explicit FileDoesNotExist(const std::string& filename) : filename_(filename) {}

    const char* what() const noexcept override {
        std::string message = "File does not exist: " + filename_;
        const char* c = message.c_str();
        return c; 
    }
private:
    std::string filename_;
};

class InvalidCommandLineArguments : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid command line arguments passed";
    }
};