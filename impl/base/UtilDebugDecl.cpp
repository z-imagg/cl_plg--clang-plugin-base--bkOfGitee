// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "base/UtilPrintAstNode.h"
#include "base/UtilDebugDecl.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/Stmt.h"
#include <clang/Frontend/CompilerInstance.h>


bool UtilDebugDecl::ifNameEqPrintSrcTxt(      FunctionDecl *funcDecl,std::string targetFuncName, ASTContext &Ctx_ref, CompilerInstance &CI_ref) {
  bool equal=false;
  IdentifierInfo *ident = funcDecl->getIdentifier();
  if(ident!= nullptr){
    const StringRef &funcName = ident->getName();
    if(!funcName.empty()){
      std::string funcNameStr=funcName.str();
      equal=(funcNameStr==targetFuncName);
      if(equal){
        std::string msg=fmt::format("找到函數名=[{}],",funcName.str());
        UtilPrintAstNode::printDecl(Ctx_ref, CI_ref, "调试",msg,funcDecl,true);
      }
    }
  }

  return equal;


}
