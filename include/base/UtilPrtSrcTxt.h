
#ifndef UtilPrtSrcTxt_H
#define UtilPrtSrcTxt_H


#include <string>

#include <clang/Rewrite/Core/Rewriter.h>
#include <set>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/AST/ParentMapContext.h>
#include <sstream>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "LocId.h"
#include "clang/Lex/Lexer.h"



#include <sstream>
#include <unordered_set>
// 放弃 c++11 string_format 宏实现、或std::sprintf实现


using namespace llvm;
using namespace clang;

class UtilPrtSrcTxt {
public:
    static void printStmt(ASTContext &Ctx, CompilerInstance &CI, std::string tag, std::string title, const  clang::Stmt *stmt,
                          bool printSourceText);
    static void printExpr(ASTContext &Ctx, CompilerInstance &CI, std::string tag, std::string title, clang::Expr *expr,
                          bool printSourceText = false);
    static void printDecl(ASTContext &Ctx, CompilerInstance &CI, std::string tag, std::string title, clang::Decl *decl,
                          bool printSourceText);
    static void  printSourceRange(int64_t nodeID,
      CompilerInstance& CI,
      std::string tag, std::string title,
      FileID fileId, const SourceRange &sourceRange,
      const char *topCategoryFieldName, const char *topCategoryName,
      const char *topCategoryEnumFieldName, int topCategoryEnum,
      const char *category1FieldName=NULL, int category1Enum=EMPTY_ENUM_VAL,
      const char *category2FieldName=NULL,int category2Enum=EMPTY_ENUM_VAL,
      bool printSourceText=false,int printSourceTextLimit=512);

    static void  printSourceRangeSimple(
            CompilerInstance& CI,
            std::string tag, std::string title,
            const SourceRange &sourceRange,
            bool printSourceText,int printSourceTextLimit=512);
    static const int EMPTY_ENUM_VAL=-1;

};


#endif //UtilPrtSrcTxt_H
