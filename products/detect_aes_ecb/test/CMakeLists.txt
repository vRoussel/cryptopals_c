add_test(NAME test_detect_aes_ecb COMMAND test.sh)
configure_file(input.txt input.txt COPYONLY)
configure_file(test.sh test.sh COPYONLY)
