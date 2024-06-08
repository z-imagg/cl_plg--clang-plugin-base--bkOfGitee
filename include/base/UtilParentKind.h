// Created by z on 2024/6/8.

#ifndef UtilParentKind_H
#define UtilParentKind_H


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

class UtilParentKind {

public:
  static bool parentIsCompound(ASTContext* astContext, const Stmt* currentStmt);

  static bool anyParentClassEqual(ASTContext* astContext, const Stmt* stmt, Stmt::StmtClass targetClass);

  static bool parentKindIsSame(ASTContext *Ctx, const Stmt* stmt, const ASTNodeKind& kind);

//若只有一个父亲，填充 父亲、父亲语句类型，返回true。否则 不填充，返回false。 Parent断言 正确与否 决定 此方法能不能被通常地使用。
static bool only1ParentNodeKind(CompilerInstance& CI, ASTContext& astContext, const Stmt* stmt , DynTypedNode& parent0, ASTNodeKind& parent0NK );
};


#endif //UtilParentKind_H
