// Created by z on 2024/6/9.

#ifndef UtilTraverseTypeDefChain_H
#define UtilTraverseTypeDefChain_H


#include <clang/AST/Type.h>
#include <clang/AST/Decl.h>

class UtilTraverseTypeDefChain {
public:
  /** 递归遍历typedef链条 直到 typedef指向的真实类型
 *
 * @param qualType_from 起点==某typedef类型别名
 * @return 终点==叶子==typedef指向的真实类型
 * 用法:
 *    clang::QualType typedef_realType = traverseTypedefChain(valueDecl->getType());
 */
  static clang::QualType traverseTypedefChain(clang::QualType qualType_from );
};


#endif //UtilTraverseTypeDefChain_H
