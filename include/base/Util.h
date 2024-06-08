//
// Created by zz on 2023/7/22.
//

#ifndef Util_H
#define Util_H


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

class Util {


};


#endif //Util_H
