project(pargen)

include(${PROJECT_SOURCE_DIR}/../BuildOpts.txt)

flex_target(lexer scan.l "${CMAKE_CURRENT_BINARY_DIR}/scan.c")
bison_target(parser parse.y "${CMAKE_CURRENT_BINARY_DIR}/parse.c")

if(CMAKE_EFFECTIVE_SYSTEM_NAME STREQUAL "Linux")
set_source_files_properties(scan.c PROPERTIES 
    COMPILE_FLAGS 
    "-Wno-sign-compare -Wno-unused-variable -Wno-implicit-function-declaration" 
)
elseif(CMAKE_EFFECTIVE_SYSTEM_NAME STREQUAL "FreeBSD")
endif()

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_BINARY_DIR}/scan.c
    ${CMAKE_CURRENT_BINARY_DIR}/parse.c
    memory.c
    hash.c
    buffer.c
    ptr_lst.c
    str.c
    str_lst.c
    ast.c
    regurg.c
    cmdline.c
    cmderrors.c
    cmdparse.c
    database.c
    main.c
)

target_link_libraries(${PROJECT_NAME}
)


