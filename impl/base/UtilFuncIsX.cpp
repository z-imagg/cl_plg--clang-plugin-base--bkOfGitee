// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "UtilFuncIsX.h"
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