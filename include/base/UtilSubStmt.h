// Created by z on 2024/6/8.

#ifndef UtilSubStmt_H
#define UtilSubStmt_H


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

class UtilSubStmt {

public:
/**计算语句列表中各语句是否为FallThrough，附带返回倒数第二条语句
 *
 * @param subStmtLs
 * @param negativeSecond
 * @return
 */
static std::vector<bool>  subStmtIsFallThroughVec(const Stmt::child_range &subStmtLs ,Stmt* &negativeSecond ,SourceManager& SM, LangOptions& langOptions);
};


#endif //UtilSubStmt_H
