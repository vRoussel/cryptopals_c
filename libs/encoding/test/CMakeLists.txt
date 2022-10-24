add_executable(test_encoding
    test_encoding.c
)
target_link_libraries(test_encoding encoding unity)
add_test(NAME test_encoding COMMAND test_encoding)
