

MESSAGE("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
#CMAKE_SOURCE_DIR: /pubx/clp-zz/

MESSAGE("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
#CMAKE_CURRENT_SOURCE_DIR: /pubx/clp-zz/tools

#导入变量 base_util__SOURCES
include("${CMAKE_CURRENT_SOURCE_DIR}/../base_home/base_util.cmake")

set(ClpBase_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/../base_home/ClpBaseMain.cpp"

"${base_util__SOURCES}"
)
message( "ClpBase_SOURCES:[${ClpBase_SOURCES}]")

add_executable(
ClpBase
${ClpBase_SOURCES}
)

target_include_directories(
ClpBase
PRIVATE
"${CMAKE_CURRENT_SOURCE_DIR}/../base_home/include"
"${LIBFMT_INCLUDE}"
)

target_link_libraries(
ClpBase
"clangTooling"
"${LIBFMT_STATIC}"  #/pubx/fmt/build/libfmt.a
)
