add_executable(decipher_xor_sbk
    main.c
    deciphered.c
)

target_link_libraries(decipher_xor_sbk
    PRIVATE crypto
    PRIVATE utils
)
