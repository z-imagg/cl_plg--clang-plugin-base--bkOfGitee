// Created by z on 2024/6/8.

#ifndef UtilGetSrcFilePathByLoc_H
#define UtilGetSrcFilePathByLoc_H


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

class UtilGetSrcFilePathByLoc {

public:
  static void getSourceFilePathOfStmt(const Stmt *S, const SourceManager &SM,StringRef& fn);

  static bool getSourceFilePathAtLoc(SourceLocation Loc, const SourceManager &SM, StringRef& fn);
};


#endif //UtilGetSrcFilePathByLoc_H
