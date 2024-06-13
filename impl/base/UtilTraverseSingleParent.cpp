// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "base/UtilTraverseSingleParent.h"
#include "base/UtilPrintAstNode.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include <clang/Frontend/CompilerInstance.h>
#include <fmt/core.h>

/**从 给定节点nodeStart向上级沿着parent[0]遍历, 沿途查找给定类别targetNodeBaseKind的子类节点, 找到则逐级返回
 *
 * @param nodeStart 给定节点
 * @param targetNodeBaseKind 给定节点基类别
 * @param targetNode_ [返回量]  查找到的匹配节点
 * @param CI
 * @param Ctx_ref
 * @return 是否匹配到给定类别的节点
 */
bool UtilTraverseSingleParent::do_traverse(clang::DynTypedNode& nodeStart, const clang::ASTNodeKind targetNodeBaseKind, clang::DynTypedNode& targetNode_/*出量*/, clang::CompilerInstance& CI, clang::ASTContext *Ctx_ref){
  const clang::DynTypedNodeList parents = Ctx_ref->getParents(nodeStart);
  int parent_cnt = parents.size();
  if(parent_cnt<=0){
    return false;
  }
  if(parent_cnt>=2){
    //若发现节点有多于1个parent ,打印告警信息
    std::string msg=fmt::format("[当心][发现节点有多于1个parent],parent个数=[{}]", parent_cnt);
    UtilPrintAstNode::printSourceRangeSimple(CI, "tag", msg, nodeStart.getSourceRange(), true, 512);
  }

  //只拿第0个parent 继续遍历
  clang::DynTypedNode parent0 = parents[0];
  const clang::ASTNodeKind parent0NodeKind = parents[0].getNodeKind();
  if(targetNodeBaseKind.isBaseOf(parent0NodeKind)){
    targetNode_=parent0;
    return true;
  }

  return UtilTraverseSingleParent::do_traverse(parent0, targetNodeBaseKind, targetNode_, CI, Ctx_ref);

}

