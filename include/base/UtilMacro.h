// Created by z on 2024/6/8.

#ifndef UtilMacro_H
#define UtilMacro_H


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

class UtilMacro {

public:
//给定位置是否在宏中
  static bool LocIsInMacro(SourceLocation Loc, SourceManager& SM);
};


#endif //UtilMacro_H
