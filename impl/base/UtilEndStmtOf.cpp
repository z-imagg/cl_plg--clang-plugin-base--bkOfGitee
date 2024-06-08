// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
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
#include "base/UtilEndStmtOf.h"

Stmt* UtilEndStmtOf::endStmtOfFunc(FunctionDecl *funcDecl) {
  Stmt *funcBody = funcDecl->getBody();
  return UtilEndStmtOf::endStmtOfCompoundStmt(funcBody);
}

Stmt* UtilEndStmtOf::endStmtOfCompoundStmt(Stmt *funcBody){
  if (funcBody && isa<CompoundStmt>(*funcBody)) {
    CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(funcBody);
    if (compoundStmt &&  (!compoundStmt->body_empty() ) ) {
      Stmt *lastStmt = compoundStmt->body_back();
      return lastStmt;
    }
  }
  return NULL;
}