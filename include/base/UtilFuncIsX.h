// Created by z on 2024/6/8.

#ifndef UtilFuncIsX_H
#define UtilFuncIsX_H


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

class UtilFuncIsX {

public:
/**
 * default函数体举例:
 * 'void func1( ) = default;'
 * 'void func2( ) {}'
 * @param funcDecl
 * @return
 */
static bool funcIsDefault(FunctionDecl *funcDecl);

/**
 * inline函数举例:
 * TODO
 * @param funcDecl
 * @return
 */
static bool funcIsInline(FunctionDecl *funcDecl);

/**
 * default构造函数体举例:
 * 'User::User( ) = default;'
 * 'User::User( ) {}'
 * @param cxxCnstrDecl
 * @return
 */
  static bool cxxConstructorIsDefault(CXXConstructorDecl *cxxCnstrDecl);

/** void函数、构造函数 最后一条语句是return吗？
 * @param funcDesc
 * @return
 */
  static bool isVoidFuncOrConstructorThenNoEndReturn(QualType funcReturnType, bool isaCXXConstructorDecl,Stmt *endStmtOfFuncBody);
};


#endif //UtilFuncIsX_H
