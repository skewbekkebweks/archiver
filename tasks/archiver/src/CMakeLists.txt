set(CMAKE_CXX_STANDARD 20)

add_executable(
        archiver
        archiver.cpp
        args_parser.cpp
        trie.cpp
        read_bit_stream.cpp
        write_bit_stream.cpp
        encode.cpp
        decode.cpp
        utility.cpp
)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)