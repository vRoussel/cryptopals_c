add_executable(decipher_xor_rk
    main.c
)

target_link_libraries(decipher_xor_rk
    PRIVATE encoding
    PRIVATE utils
)

add_subdirectory(test)
