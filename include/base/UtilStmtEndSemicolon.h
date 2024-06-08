// Created by z on 2024/6/8.

#ifndef UtilStmtEndSemicolon_H
#define UtilStmtEndSemicolon_H


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

class UtilStmtEndSemicolon {

public:
//获取语句末尾分号位置
static SourceLocation getStmtEndSemicolonLocation(const Stmt *S, const SourceManager &SM,bool& endIsSemicolon)  ;
};


#endif //UtilStmtEndSemicolon_H
