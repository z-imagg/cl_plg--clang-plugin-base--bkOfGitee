// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
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
#include "base/UtilLineNum.h"

void UtilLineNum::extractLineAndColumn(const SourceManager& SM, const SourceLocation& sourceLocation, int& line, int& column) {
  PresumedLoc presumedLoc = SM.getPresumedLoc(sourceLocation);
  line = presumedLoc.getLine();
  column = presumedLoc.getColumn();
  return;
}

bool UtilLineNum::isEqSrcLocLineNum(const SourceManager& SM, const SourceLocation& srcLoc1, const SourceLocation& srcLoc2) {
  PresumedLoc presumedLoc1 = SM.getPresumedLoc(srcLoc1);
  PresumedLoc presumedLoc2 = SM.getPresumedLoc(srcLoc2);
  int line1 = presumedLoc1.getLine();
  int line2 = presumedLoc2.getLine();
  bool lineNumEqual=(line1==line2);
  return lineNumEqual;
}