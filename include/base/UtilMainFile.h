// Created by z on 2024/6/8.

#ifndef UtilMainFile_H
#define UtilMainFile_H


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

class UtilMainFile {

public:
//在给定声明列表中，找到第一个在MainFile的声明
static Decl* firstDeclInMainFile(SourceManager&SM, std::vector<Decl*> declVec);

//判断给定声明是否在主文件中
static bool isDeclInMainFile(SourceManager&SM, Decl* D);

  static void getMainFileIDMainFilePath(SourceManager& SM, FileID& mainFileId, std::string& mainFilePath);

  static bool LocFileIDEqMainFileID(SourceManager& SM, SourceLocation Loc);
};


#endif //UtilMainFile_H
