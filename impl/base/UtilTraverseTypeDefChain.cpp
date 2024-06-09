// 遍历 typedef 链条


#include "base/UtilTraverseTypeDefChain.h"

#include <clang/AST/Type.h>
#include <clang/AST/Decl.h>
#include <fmt/core.h>
#include <iostream>

//递归遍历typedef链条 直到 typedef指向的真实类型
clang::QualType UtilTraverseTypeDefChain::traverseTypedefChain(clang::QualType qualType_from) {
  const clang::Type* typePtr_from = qualType_from.getTypePtr();

  // 如果是typedef
  if (const clang::TypedefType* typedefType_from = llvm::dyn_cast<clang::TypedefType>(typePtr_from)) {
    clang::TypedefNameDecl* typedefNameDecl_from = typedefType_from->getDecl();

    // typedef指向的类型
    clang::QualType typedef_QualType_to = typedefNameDecl_from->getUnderlyingType();

    //{打印调试信息
    const std::string typedef_fromName = typedefNameDecl_from->getNameAsString();
    const std::string typedef_toName = typedef_QualType_to.getAsString();
//    std::string msg=fmt::format("[typedef链条遍历步] typedef_fromName={}, typedef_toName={}\n", typedef_fromName, typedef_toName);
//    std::cout<<msg;
    //}

    // 递归,
    //    要递归的理由 是typedef指向的类型 可能继续是typedef 即有多层typedef
    return traverseTypedefChain(typedef_QualType_to);
  }


  // 如果不是typedef 说明已经深入到了叶子 此即为typedef指向的真实类型
  return qualType_from;
}