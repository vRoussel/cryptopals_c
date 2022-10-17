add_executable(test_utils
    test_utils.c
    ../utils.c
)
target_link_libraries(test_utils unity)
add_test(NAME test_utils COMMAND test_utils)
