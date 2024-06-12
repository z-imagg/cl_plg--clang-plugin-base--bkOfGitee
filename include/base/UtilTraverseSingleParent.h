
#ifndef UtilTraverseSingleParent_H
#define UtilTraverseSingleParent_H


#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTTypeTraits.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

// clang::DynTypedNode node=clang::DynTypedNode::create(*stmt);
class UtilTraverseSingleParent {
public:
  /**从 给定节点nodeStart向上级沿着parent[0]遍历, 沿途查找给定类别targetNodeKind节点, 找到则逐级返回
 * @param nodeStart 给定节点
 * @param targetNodeKind 给定节点类别
 * @param targetNode_ [返回量]  查找到的匹配节点
 * @param CI
 * @param astContext
 * @return 是否匹配到给定类别的节点
 * 使用举例
     clang::DynTypedNode node=clang::DynTypedNode::create(*stmt);
     bool found=UtilTraverseSingleParent::do_traverse(node,...);
 */
  static bool do_traverse(clang::DynTypedNode& nodeStart, const clang::ASTNodeKind targetNodeKind, clang::DynTypedNode& targetNode_/*出量*/, clang::CompilerInstance& CI, clang::ASTContext *Ctx_ref );
};


#endif //UtilTraverseSingleParent_H
