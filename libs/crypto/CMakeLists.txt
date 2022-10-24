add_library(crypto
    crypto.c
    languages.c
)

target_link_libraries(crypto
    PRIVATE encoding
    PRIVATE utils
)

add_subdirectory(test)
