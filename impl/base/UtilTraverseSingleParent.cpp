// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "base/UtilTraverseSingleParent.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

bool UtilTraverseSingleParent::traverseSingleParent(clang::ASTContext *astContext, clang::DynTypedNode node){
  const clang::DynTypedNodeList parents = astContext->getParents(*stmt);

  if(parents.size()!=1){
    return false;
  }
  /*DynTypedNode */clang::DynTypedNode parent0 = parents[0];
  const clang::ASTNodeKind parent0NK = parents[0].getNodeKind();

  clang::DynTypedNode node=clang::DynTypedNode::create(*stmt);

  UtilTraverseSingleParent::traverseSingleParent(*astContext, parent0);

  return false;
}

