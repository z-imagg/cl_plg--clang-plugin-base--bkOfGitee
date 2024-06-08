// Created by z on 2024/6/8.

#ifndef UtilPrintAstNode_H
#define UtilPrintAstNode_H


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

class UtilPrintAstNode {

public:
  static const int EMPTY_ENUM_VAL=-1;

  static void printStmt(ASTContext &Ctx, CompilerInstance &CI, std::string tag, std::string title, const  Stmt *stmt,
                        bool printSourceText);

  static void printExpr(ASTContext &Ctx, CompilerInstance &CI, std::string tag, std::string title, Expr *expr,
                        bool printSourceText = false);

  static void printDecl(ASTContext &Ctx, CompilerInstance &CI, std::string tag, std::string title, Decl *decl,
                        bool printSourceText);

  static void  printSourceRange(int64_t nodeID,
                                CompilerInstance& CI,
                                std::string tag, std::string title,
                                FileID fileId, const SourceRange &sourceRange,
                                const char *topCategoryFieldName, const char *topCategoryName,
                                const char *topCategoryEnumFieldName, int topCategoryEnum,
                                const char *category1FieldName=NULL, int category1Enum=EMPTY_ENUM_VAL,
                                const char *category2FieldName=NULL, int category2Enum=EMPTY_ENUM_VAL,
                                bool printSourceText=false, int printSourceTextLimit=512);

  static void  printSourceRangeSimple(
            CompilerInstance& CI,
            std::string tag, std::string title,
            const SourceRange &sourceRange,
            bool printSourceText,int printSourceTextLimit=512);
};


#endif //UtilPrintAstNode_H
