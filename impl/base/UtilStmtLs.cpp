// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "UtilStmtLs.h"
#include "base/UtilAttrKind.h"
#include "base/UtilRetStmt.h"
#include "base/UtilSrcRangeRelation.h"
#include "base/UtilNextToken.h"
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
#include "base/UtilStmtLs.h"

std::vector<std::string> UtilStmtLs::stmtLs2TextLs(std::vector<Stmt*> stmtVec, SourceManager & SM, const LangOptions & langOptions){
  std::vector<std::string> textVec;

  std::transform(stmtVec.begin(), stmtVec.end(), std::back_inserter(textVec), [&SM,&langOptions](Stmt* stmt) {
      return UtilGetSrcTxtBySrcRange::getSourceTextBySourceRange(stmt->getSourceRange(),SM,langOptions); // 这里可以根据需要进行转换操作
  });

  return textVec;
}