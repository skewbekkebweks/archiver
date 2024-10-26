#include "args_parser.h"
#include "encode.h"
#include "decode.h"

#include <iostream>
#include <string>
#include <vector>

const int SUCCESS_EXIT_CODE = 0;
const int FAILED_EXIT_CODE = 111;

const std::string ENCODE_FLAG = "-c";
const std::string DECODE_FLAG = "-d";
const std::string HELP_FLAG = "-h";

void PrintHelp() {
    std::cout
        << "Usage: archiver [OPTIONS]\n"
        << "Options:\n"
        << "  -c, --create ARCHIVE_NAME FILE1 [FILE2 ...]  Create an archive with the specified name containing the "
           "specified files.\n"
        << "      - ARCHIVE_NAME : Name of the output archive file.\n"
        << "      - FILE1, FILE2, ... : List of files to archive.\n"
        << "  -d, --decompress ARCHIVE_NAME                   Decompress the specified archive, extracting its "
           "contents to the current directory.\n"
        << "      - ARCHIVE_NAME : Name of the archive file to decompress.\n"
        << "  -h, --help                                       Display this help message and exit.\n"
        << "Examples:\n"
        << "  archiver -c my_archive.zip file1.txt file2.txt\n"
        << "  archiver -d my_archive.zip\n"
        << "  archiver --help\n"
        << "Note: Ensure that you have permission to read the input files and write to the desired output location.\n";
}

int main(int argc, char* argv[]) {
    try {
        ArgsParser parser{argc, argv};

        if (parser.HasArg(ENCODE_FLAG)) {
            std::vector<std::string> args = parser.GetArgValues(ENCODE_FLAG);
            if (args.empty()) {
                throw ArchiveNameNotPassed{};
            }

            std::string archive_name = args[0];
            if (args.size() < 2) {
                throw FilenameNotPassed{};
            }

            std::vector<std::string> file_names{args.begin() + 1, args.end()};

            Encode(archive_name, file_names);

        } else if (parser.HasArg(DECODE_FLAG)) {
            std::vector<std::string> args = parser.GetArgValues(DECODE_FLAG);
            if (args.empty()) {
                throw ArchiveNameNotPassed{};
            }
            std::string archive_name = args[0];

            Decode(archive_name);
        } else if (parser.HasArg(HELP_FLAG)) {
            PrintHelp();
        } else {
            throw InvalidCommandLineArgumentsError{};
        }
    } catch (std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return FAILED_EXIT_CODE;
    }

    return SUCCESS_EXIT_CODE;
}