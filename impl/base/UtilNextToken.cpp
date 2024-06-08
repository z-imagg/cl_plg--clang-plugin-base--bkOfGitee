// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "UtilNextToken.h"
#include "base/UtilConvertNodeType.h"
#include "base/UtilSubStmt.h"
#include "base/UtilDiagnostics.h"
#include "base/UtilFuncDecl.h"
#include "base/UtilEnvVar.h"
#include "base/UtilFile.h"
#include "base/UtilStmtEndSemicolon.h"
#include "base/UtilIsSysSrcFileOrMe.h"
#include "base/UtilLocId.h"
#include "base/UtilMainFile.h"
#include "base/UtilLineNum.h"
#include "base/UtilCompoundStmt.h"
#include "base/UtilRewriteBuffer.h"
#include "base/UtilFuncIsX.h"
#include "base/UtilEndStmtOf.h"
#include "base/UtilInsertInclude.h"
#include "base/UtilPrintAstNode.h"
#include "base/UtilGetSrcTxtBySrcRange.h"
#include "base/UtilGetSrcFilePathByLoc.h"
#include "base/UtilParentKind.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include "base/Util.h"
#include "base/UtilNextToken.h"

//从给定位置的Token移动到下一个Token所得的位置。 由于switch语句中冒号下一个Token位置的奇怪结果，导致此方法 是否在任何情况下都能实现 移动到下一个位置 有待确定
SourceLocation UtilNextToken::nextTokenLocation(SourceLocation thisTokenLoc, const SourceManager& SM, const LangOptions& LO, int offset){
//    switch语句中冒号下一个Token位置的奇怪结果:
//    beginLoc = Lexer::getLocForEndOfToken(SwitchCase->getColonLoc(), /*Offset*/0, SM, LO);//offset:0 所取得位置是冒号右边 是期望位置
//    beginLoc = Lexer::getLocForEndOfToken(SwitchCase->getColonLoc(), /*Offset*/1, SM, LO);//offset:1 所取得位置是冒号左边 非期望位置
//按道理说 offset:1 冒号右边  ，offset:0 冒号左边 才对， 但结果是反过来的。不知道如何解释，据说是因为 冒号这个小Token 所在位置 同时 被一个更大的Token占据，导致offset:1 是 相对于 此大Token的，但这个解释有点勉强。
  //以下2步骤，确定能移动到下一个token位置？
  Token thisToken;
  //步骤1. 取给定位置的Token, 忽略空格、tab等空白符
  Lexer::getRawToken(thisTokenLoc, thisToken, SM, LO,/*IgnoreWhiteSpace:*/true);
  //步骤2. 相对 该Token的结束位置 右移1个Token 所获得的位置 即 下一个token位置
  const SourceLocation nextTokenLoc = Lexer::getLocForEndOfToken(thisToken.getEndLoc(), /*向右移1个Token*/offset, SM, LO);
  return nextTokenLoc;
}