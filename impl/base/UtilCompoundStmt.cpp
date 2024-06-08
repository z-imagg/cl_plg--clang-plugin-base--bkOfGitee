// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "UtilCompoundStmt.h"
#include "base/UtilRewriteBuffer.h"
#include "base/UtilFuncIsX.h"
#include "base/UtilEndStmtOf.h"
#include "base/UtilInsertInclude.h"
#include "base/UtilPrintAstNode.h"
#include "base/UtilGetSrcTxtBySrcRange.h"
#include "base/UtilGetSrcFilePathByLoc.h"
#include "base/UtilParentKind.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include "base/Util.h"
#include "base/UtilCompoundStmt.h"

int UtilCompoundStmt::childrenCntOfCompoundStmt(CompoundStmt* stmt){
  if(!stmt){
    return 0;
  }
//  int cnt=std::distance(stmt->child_begin(),stmt->child_end());
  return stmt->size();
}

bool UtilCompoundStmt::GetCompoundLRBracLoc(CompoundStmt*& compoundStmt, SourceLocation& funcBodyLBraceLoc, SourceLocation& funcBodyRBraceLoc){
  if( compoundStmt ){
    funcBodyLBraceLoc = compoundStmt->getLBracLoc();
    funcBodyRBraceLoc = compoundStmt->getRBracLoc();
    return true;
  }
  return false;
}

/**
 *
 * @param funcBody
 * @param funcBodyLBraceLoc
 * @return 若是组合语句(CompoundStmt) ，则取左花括号位置
 */
bool UtilCompoundStmt::funcBodyIsCompoundThenGetLRBracLoc(Stmt *funcBody, CompoundStmt*& compoundStmt, SourceLocation& funcBodyLBraceLoc, SourceLocation& funcBodyRBraceLoc){
  if( compoundStmt = dyn_cast<CompoundStmt>(funcBody)){
    funcBodyLBraceLoc = compoundStmt->getLBracLoc();
    funcBodyRBraceLoc = compoundStmt->getRBracLoc();
    return true;
  }
  return false;
}

bool UtilCompoundStmt::isLastCompoundStmt(CompoundStmt *stmt, ASTContext &context) {
  auto parents = context.getParents(*stmt);

  // 遍历父节点列表
  for (auto it = parents.begin(); it != parents.end(); ++it) {
    if (const FunctionDecl *func = it->get<FunctionDecl>()) {
      // 检查CompoundStmt是否为最后一个块
      Stmt *body = func->getBody();
      if (body && body == stmt) {
        return true;
      }
    } else if (const LambdaExpr *lambda = it->get<LambdaExpr>()) {
      // 检查CompoundStmt是否为lambda表达式的最后一个块
      Stmt *body = lambda->getBody();
      if(body){
        if (CompoundStmt *lambdaBody = dyn_cast<CompoundStmt>(body)) {
          Stmt *lastStmt = lambdaBody->body_back();
          if (lastStmt && lastStmt == stmt) {
            return true;
          }
        }
      }

    }
  }

  return false;
}

FunctionDecl *UtilCompoundStmt::getContainingFunction(CompoundStmt *stmt, ASTContext &context) {
  auto parents = context.getParents(*stmt);

  // 遍历父节点列表
  for (auto itJ = parents.begin(); itJ != parents.end(); ++itJ) {
    if (const LambdaExpr *lambdaJ = itJ->get<LambdaExpr>()) {
      // 返回包裹CompoundStmt的lambda
      CXXMethodDecl *methodJ = lambdaJ->getCallOperator();
      if (methodJ) {
        return methodJ;
      }
    } else if (const FunctionDecl *funcJ = itJ->get<FunctionDecl>()) {
      // 返回最近的FunctionDecl
      return const_cast<FunctionDecl*>(funcJ);
    }
  }

  return nullptr;
}