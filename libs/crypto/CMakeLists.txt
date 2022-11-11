find_package(OpenSSL REQUIRED)

add_library(crypto
    crypto.c
    languages.c
)

target_link_libraries(crypto
    PRIVATE encoding
    PRIVATE utils
    PRIVATE OpenSSL::Crypto
)

add_subdirectory(test)
