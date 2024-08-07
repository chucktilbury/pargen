project(pargen)

include(${PROJECT_SOURCE_DIR}/../BuildOpts.txt)

flex_target(lexer scan.l "${CMAKE_CURRENT_BINARY_DIR}/scan.c")
bison_target(parser parse.y "${CMAKE_CURRENT_BINARY_DIR}/parse.c")

get_filename_component(CCOMPILER ${CMAKE_C_COMPILER} NAME)

message("${CCOMPILER}")
if(CCOMPILER STREQUAL "gcc")
set_source_files_properties(scan.c PROPERTIES 
    COMPILE_FLAGS 
    "-Wno-sign-compare -Wno-unused-variable -Wno-implicit-function-declaration" 
)
elseif(CCOMPILER STREQUAL "clang")
set_source_files_properties(scan.c PROPERTIES 
    COMPILE_FLAGS 
    "-Wno-sign-compare -Wno-unused-variable -Wno-implicit-function-declaration -Wno-parentheses-equality" 
)
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
    emit.c
    emit_parser.c
    emit_ast.c
    main.c
)

target_link_libraries(${PROJECT_NAME}
)


