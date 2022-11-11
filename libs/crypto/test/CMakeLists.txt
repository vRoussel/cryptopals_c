add_executable(test_crypto
    test_crypto.c
)
target_link_libraries(test_crypto crypto unity)
add_test(NAME test_crypto COMMAND test_crypto)
configure_file(test_decipher_aes_128_ecb_input.txt test_decipher_aes_128_ecb_input.txt COPYONLY)
configure_file(test_decipher_aes_128_ecb_output.txt test_decipher_aes_128_ecb_output.txt COPYONLY)
