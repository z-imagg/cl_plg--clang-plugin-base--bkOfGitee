// Created by z on 2024/6/8.

#include "base/UtilFuncIsX.h"
#include "base/UtilEnvVar.h"
#include "base/UtilFile.h"
#include "base/UtilStmtEndSemicolon.h"
#include "base/UtilIsSysSrcFileOrMe.h"
#include "base/UtilLocId.h"
#include "base/UtilMainFile.h"
#include "base/UtilLineNum.h"
#include "base/UtilCompoundStmt.h"
#include "base/UtilRewriteBuffer.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
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
#include "base/UtilFuncIsX.h"
#include "UtilRetStmt.h"

bool UtilFuncIsX::funcIsDefault(FunctionDecl *funcDecl){
  bool isDefault=funcDecl->isExplicitlyDefaulted() || funcDecl->isDefaulted();
  return isDefault;
}

bool UtilFuncIsX::funcIsInline(FunctionDecl *funcDecl){

    bool isInline=
            funcDecl->isInlined()
            || funcDecl->isInlineSpecified()
            ;

    //TODO 以下两个是否能作为inline的一种，不确定
//  funcDecl->isInlineBuiltinDeclaration();
//  funcDecl->isInlineNamespace();

    return isInline;
}

bool UtilFuncIsX::cxxConstructorIsDefault(CXXConstructorDecl *cxxCnstrDecl){
  bool isDefault= cxxCnstrDecl->isExplicitlyDefaulted() || cxxCnstrDecl->isDefaulted() || cxxCnstrDecl->isDefaultConstructor();
  return isDefault;
}

/** void函数、构造函数 最后一条语句是return吗？
 * @param funcDesc
 * @return
 */
bool UtilFuncIsX::isVoidFuncOrConstructorThenNoEndReturn(QualType funcReturnType, bool isaCXXConstructorDecl, Stmt *endStmtOfFuncBody){
  //void函数、构造函数 最后一条语句若不是return，则需在最后一条语句之后插入  函数释放语句
  if(funcReturnType->isVoidType() || isaCXXConstructorDecl){
    //是void函数 或是 构造函数: 此两者都可以末尾不显示写出return语句
//    Stmt *endStmtOfFuncBody = funcDesc.endStmtOfFuncBody;
    bool endStmtIsReturn=Util::isReturnStmtClass(endStmtOfFuncBody);
    if(!endStmtIsReturn){
      return true;
    }
  }
  return false;
}