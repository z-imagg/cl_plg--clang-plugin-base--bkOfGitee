// Created by z on 2024/6/8.

#ifndef UtilConvertNodeType_H
#define UtilConvertNodeType_H


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

class UtilConvertNodeType {

public:
/**
 * 父亲节点们 转为 指定类型 的 节点列表，
 *     指定类型 不合适， 即  若父亲i的类型  不是  指定类型，  则返回向量中i位置为NULL，
 *            可以通过返回向量中i位置的ASTNodeKind确定给定何类型合适。
 * @tparam ParentNodeTp 指定类型
 * @param parents 输入的 父亲节点们
 * @param parentVec 返回 元组<父亲i的ASTNodeKind,父亲i转为给定类型的结果> 向量
 */
    template<typename ParentNodeTp>
    static void collectParentS(const DynTypedNodeList &parents ,std::vector<std::tuple<ASTNodeKind,SourceRange,const ParentNodeTp*>> & parentVec){
//      const DynTypedNodeList &parents = ctx.getParents(*stmt);

      for (const DynTypedNode& parentNodeI : parents) {
        //注意  :  parentNodeI.get<T>():  如果parentNode不是类型T的，此get返回NULL
        const ParentNodeTp *parentI = parentNodeI.get<ParentNodeTp>();
        SourceRange parentISourceRange=parentNodeI.getSourceRange();

        const ASTNodeKind &parentINodeKind = parentNodeI.getNodeKind();
        parentVec.push_back(std::make_tuple(parentINodeKind, parentISourceRange,parentI));
      }
    }
};


#endif //UtilConvertNodeType_H
