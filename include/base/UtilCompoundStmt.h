// Created by z on 2024/6/8.

#ifndef UtilCompoundStmt_H
#define UtilCompoundStmt_H


#include <unordered_set>
#include "clang/Lex/Lexer.h"
#include "LocId.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include <sstream>
#include <clang/AST/ParentMapContext.h>
#include <clang/Frontend/CompilerInstance.h>
#include <set>
#include <clang/Rewrite/Core/Rewriter.h>
#include <string>

class UtilCompoundStmt {

public:
  static int childrenCntOfCompoundStmt(CompoundStmt* stmt);

  static bool GetCompoundLRBracLoc(CompoundStmt*& compoundStmt, SourceLocation& funcBodyLBraceLoc, SourceLocation& funcBodyRBraceLoc);

/**
 *
 * @param funcBody
 * @param funcBodyLBraceLoc
 * @return 是否组合语句,即是否CompoundStmt
 */
static bool funcBodyIsCompoundThenGetLRBracLoc(Stmt *funcBody, CompoundStmt*& compoundStmt, SourceLocation& funcBodyLBraceLoc, SourceLocation& funcBodyRBraceLoc);

  static bool isLastCompoundStmt(CompoundStmt *stmt, ASTContext &context);

  static FunctionDecl *getContainingFunction(CompoundStmt *stmt, ASTContext &context);
};


#endif //UtilCompoundStmt_H
