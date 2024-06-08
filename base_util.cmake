
#用法      include("/fridaAnlzAp/clp-zz/base_home/base_util.cmake")
#      或 include("${CMAKE_CURRENT_SOURCE_DIR}/../base_home/base_util.cmake")

set(base_util__SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/Util.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilParentKind.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilGetSrcFilePathByLoc.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilGetSrcTxtBySrcRange.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilPrintAstNode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilInsertInclude.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilEndStmtOf.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilFuncIsX.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilRewriteBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilCompoundStmt.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilLineNum.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilMainFile.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilLocId.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/UtilIsSysSrcFileOrMe.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/ASTContextUtil.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/StrUtil.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/LocId.cpp"
)