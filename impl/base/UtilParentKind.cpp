// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "base/UtilParentKind.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include "base/Util.h"
#include "base/UtilParentKind.h"

bool UtilParentKind::parentIsCompound(ASTContext* astContext, const Stmt* currentStmt) {
  bool parentKindIsCompound= UtilParentKind::parentKindIsSame(astContext, currentStmt, ASTNodeKind::getFromNodeKind<CompoundStmt>());
  bool parentClassIsCompound= UtilParentKind::anyParentClassEqual(astContext, currentStmt, Stmt::CompoundStmtClass);
  bool parentIsCompound=parentKindIsCompound||parentClassIsCompound;
  return parentIsCompound;
}

//原名:parentClassEqual ，现名:anyParentClassEqual
bool UtilParentKind::anyParentClassEqual(ASTContext* astContext, const Stmt* stmt, Stmt::StmtClass targetClass) {
  auto parents = astContext->getParents(*stmt);

  for (const auto& parent : parents) {
    auto stmtParent = parent.get<Stmt>();
    if (stmtParent && stmtParent->getStmtClass() == targetClass) {
      return true;
    }
  }

  return false;
}

bool UtilParentKind::parentKindIsSame(ASTContext *Ctx, const Stmt* stmt, const ASTNodeKind& kind){
  if(!Ctx || !stmt){
    return false;
  }
  DynTypedNodeList parentS=Ctx->getParents(*stmt);
  for (const auto& parent : parentS) {
    if (   kind.isSame(parent.getNodeKind())  ) {
      return true;
    }
  }

  return false;
}

//若只有一个父亲，填充 该父亲节点的语句类型，返回true。否则 不填充，返回false。
bool UtilParentKind::only1ParentNodeKind(CompilerInstance& CI, ASTContext& astContext, const Stmt* stmt , DynTypedNode& parent0, ASTNodeKind& parent0NK ) {
  SourceManager &SM = CI.getSourceManager();
  auto parents = astContext.getParents(*stmt);

  if(parents.size()!=1){
    return false;
  }
  /*DynTypedNode */parent0 = parents[0];
  parent0NK=parents[0].getNodeKind();

  return true;

}