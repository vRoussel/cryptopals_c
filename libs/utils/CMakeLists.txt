add_library(utils
    utils.c
    hashmap.c
)

target_link_libraries(utils
    crypto
)

add_subdirectory(test)
