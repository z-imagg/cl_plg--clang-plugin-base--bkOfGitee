// Created by z on 2024/6/8.

#ifndef UtilInsertInclude_H
#define UtilInsertInclude_H


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

class UtilInsertInclude {

public:
  static void insertIncludeToFileStartByLoc(StringRef includeStmtText,SourceLocation Loc, SourceManager &SM, const std::shared_ptr<Rewriter> rewriter_ptr);

  static void insertIncludeToFileStart(StringRef includeStmtText, FileID fileId, SourceManager &SM, const std::shared_ptr<Rewriter> rewriter_ptr, bool& insertResult);

  static void insertCommentBeforeLoc(StringRef commentText, SourceLocation Loc, const std::shared_ptr<Rewriter> mRewriter_ptr, bool& insertResult);
};


#endif //UtilInsertInclude_H
