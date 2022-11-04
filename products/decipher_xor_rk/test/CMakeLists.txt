add_test(NAME test_decipher_xor_rk_product COMMAND test.sh)
configure_file(input.txt input.txt COPYONLY)
configure_file(test.sh test.sh COPYONLY)
