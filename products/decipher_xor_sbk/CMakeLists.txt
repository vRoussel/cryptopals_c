add_executable(decipher_xor_sbk
    main.c
)

target_link_libraries(decipher_xor_sbk
    PRIVATE crypto
    PRIVATE encoding
    PRIVATE utils
)

add_subdirectory(test)
