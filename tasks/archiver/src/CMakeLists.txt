set(CMAKE_CXX_STANDARD 20)

add_executable(
        archiver
        archiver.cpp
        args_parser.h
        exception.h
        bit_stream.h
)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)