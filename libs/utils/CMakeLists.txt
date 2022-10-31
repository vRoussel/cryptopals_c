add_library(utils
    utils.c
)

target_link_libraries(utils
    crypto
)

add_subdirectory(test)
