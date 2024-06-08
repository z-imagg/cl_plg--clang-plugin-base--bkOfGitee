// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
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
#include "base/UtilSubStmt.h"
#include "base/UtilAttrKind.h"

std::vector<bool>  UtilSubStmt::subStmtIsFallThroughVec(const Stmt::child_range &subStmtLs , Stmt* &negativeSecond, SourceManager& SM, LangOptions& langOptions) {
  std::vector<Stmt*> subStmtVec(subStmtLs.begin(), subStmtLs.end());
  unsigned long subStmtCnt = subStmtVec.size();
  const std::vector<std::string> &textVec = Util::stmtLs2TextLs(subStmtVec, SM, langOptions);
  if(subStmtCnt>=2){
    //倒数第二条语句
    negativeSecond=subStmtVec[subStmtCnt-2];
  }

  //subStmtVec中的stmtJ是否为'[[gnu::fallthrough]];'
  std::vector<bool> subStmtIsFallThroughVec(subStmtCnt,false);

  for (std::size_t j = 0; j < subStmtCnt; ++j) {
    Stmt* stmtJ = subStmtVec[j];
    subStmtIsFallThroughVec[j]=UtilAttrKind::hasAttrKind(stmtJ, attr::FallThrough);
      //如果本行语句是'[[gnu::fallthrough]];'  , 那么下一行前不要插入时钟语句, 否则语法错误.
  }
  return subStmtIsFallThroughVec;
}