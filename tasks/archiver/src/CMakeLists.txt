set(CMAKE_CXX_STANDARD 20)

add_executable(
        archiver
        archiver.cpp
        args_parser.h
        args_parser.cpp
        exception.h
        bit_stream.h
        bit_stream.cpp
        trie.h
        trie.cpp
)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)