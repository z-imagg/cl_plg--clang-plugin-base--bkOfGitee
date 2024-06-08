
#用法      include("/fridaAnlzAp/clp-zz/base_home/base_util.cmake")
#      或 include("${CMAKE_CURRENT_SOURCE_DIR}/../base_home/base_util.cmake")

set(base_util__SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/Util.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilSrcRange.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilPrtSrcTxt.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilParentKind.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/ASTContextUtil.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/StrUtil.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/LocId.cpp"
)

message( "base_util__SOURCES:[${base_util__SOURCES}]")
