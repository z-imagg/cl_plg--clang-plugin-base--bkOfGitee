// Created by z on 2024/6/8.

#ifndef UtilRewriteBuffer_H
#define UtilRewriteBuffer_H


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

class UtilRewriteBuffer {

public:
  static std::string rewriteBufferToString(const RewriteBuffer &buffer);

  static void
  saveRewriteBuffer(const std::shared_ptr<Rewriter> rewriter_ptr, FileID mainFileId, std::string filePath);

  static void saveRewriteBuffer0(const RewriteBuffer *pRewriteBuffer, std::string filePath, std::string title);
};


#endif //UtilRewriteBuffer_H
