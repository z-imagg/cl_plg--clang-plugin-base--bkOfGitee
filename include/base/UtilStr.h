// Created by z on 2024/6/8.

#ifndef UtilStr_H
#define UtilStr_H


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

class UtilStr {

public:
  static void emptyStrIfNullStr(const char* &cstr);

/** c++11 手工实现 string_format
   * 模板，必须要放在头文件中.
   * @tparam Args
   * @param format
   * @param args
   * @return
   */



  static std::string pointerToString(void* ptr);
};


#endif //UtilStr_H
