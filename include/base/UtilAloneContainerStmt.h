// Created by z on 2024/6/8.

#ifndef UtilAloneContainerStmt_H
#define UtilAloneContainerStmt_H


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

class UtilAloneContainerStmt {

public:
/**是否 独立且容器 语句
 * 所谓 容器 即 能容纳别的语句 的语句
 * 所以 独立 即 能独立存在 而不会报语法错误 的语句
 *
 *注意 else 是容器语句，但else不能独立存在，因此 else 不是 独立且容器 语句
 *   else必须依附if才能存在
 * 块、if、for、while、do-while  是  独立且容器 语句
 * @param stmt
 * @return
 */
  static bool isAloneContainerStmt(const Stmt *stmt)  ;
};


#endif //UtilAloneContainerStmt_H
