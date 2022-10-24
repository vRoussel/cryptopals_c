add_executable(test_crypto
    test_crypto.c
)
target_link_libraries(test_crypto crypto unity)
add_test(NAME test_crypto COMMAND test_crypto)
