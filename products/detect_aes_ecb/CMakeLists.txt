add_executable(detect_aes_ecb
    main.c
)

target_link_libraries(detect_aes_ecb
    crypto
    encoding
    utils
)

add_subdirectory(test)
