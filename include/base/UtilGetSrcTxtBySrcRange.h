// Created by z on 2024/6/8.

#ifndef UtilGetSrcTxtBySrcRange_H
#define UtilGetSrcTxtBySrcRange_H


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

class UtilGetSrcTxtBySrcRange {

public:
  static std::string getSourceTextBySourceRange(SourceRange sourceRange, SourceManager & sourceManager, const LangOptions & langOptions);

  static std::tuple<std::string,std::string>  get_FileAndRange_SourceText(const SourceRange &sourceRange, CompilerInstance& CI);
};


#endif //UtilGetSrcTxtBySrcRange_H
