// Created by z on 2024/6/8.

#ifndef UtilLineNum_H
#define UtilLineNum_H


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

class UtilLineNum {

public:
  static void extractLineAndColumn(const SourceManager& SM, const SourceLocation& sourceLocation, int& line, int& column);

/** 两给定'SourceLocation'的行号是否相同
 *
 * @param SM
 * @param srcLoc1
 * @param srcLoc2
 * @return
 */
static bool isEqSrcLocLineNum(const SourceManager& SM, const SourceLocation& srcLoc1, const SourceLocation& srcLoc2);
};


#endif //UtilLineNum_H
