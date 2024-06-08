// Created by z on 2024/6/8.

#ifndef UtilDiffSysSrcFileAndMe_H
#define UtilDiffSysSrcFileAndMe_H


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

class UtilIsSysSrcFileOrMe {

public:
/**给定源文件路径是否系统源文件
* 系统源文件路径举例：
/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/cpp_type_traits.h
/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/ext/type_traits.h
/usr/include/x86_64-linux-gnu/bits/iscanonical.h

/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include/uintrintrin.h
* @param fn
* @return
*/
static bool isSysSrcFile(StringRef fn);

  static bool isRuntimeSrcFile(StringRef fn, std::string runtimeBaseName) ;
};


#endif //UtilDiffSysSrcFileAndMe_H
