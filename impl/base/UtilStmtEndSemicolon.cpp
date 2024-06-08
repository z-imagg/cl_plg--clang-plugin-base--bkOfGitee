// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "UtilStmtEndSemicolon.h"
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
#include "base/UtilStmtEndSemicolon.h"

//获取语句末尾分号位置
SourceLocation UtilStmtEndSemicolon::getStmtEndSemicolonLocation(const Stmt *S, const SourceManager &SM, bool& endIsSemicolon) {
  const LangOptions &LO = LangOptions();
  Token JTok;

  // 获取Stmt的结束位置
  SourceLocation JLoc = S->getEndLoc();
  if(JLoc.isInvalid()){
    //如果语句末尾位置 就不合法，则标记没找到分号，再直接返回。
    endIsSemicolon= false;
    return JLoc;
  }

  do{

    Lexer::getRawToken(JLoc, JTok, SM, LO,/*IgnoreWhiteSpace:*/true);
    //忽略空白字符，IgnoreWhiteSpace：true，很关键，否则可能某个位置导致循环后还是该位置，从而死循环。
    JLoc = Lexer::getLocForEndOfToken(JTok.getEndLoc(), /*Offset*/1, SM, LO);
    //偏移量给1,Offset：1,很关键，如果不向前移动 可能循环一次还是在该位置，造成死循环。
    //取第J次循环的Token的结尾位置，JTok.getEndLoc()，很关键，否则可能下次循环还在该token上，导致死循环。
  }while (JTok.isNot(tok::semi)
          && JTok.isNot(tok::eof)
          && JTok.getLocation().isValid()
          );


  // 获取分号的结束位置

  endIsSemicolon=JTok.is(tok::semi);
  return JTok.getLocation();
}