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

int UtilDebugDecl::funcDecl_GlobalCounter=0;
void UtilDebugDecl::debugFuncDeclByGlobalCounter(      FunctionDecl *funcDecl, ASTContext &Ctx_ref, CompilerInstance &CI_ref) {
  funcDecl_GlobalCounter++;
  // 调试步骤(函数声明全局计数器条件断点):
  // 1.  非调试地运行一次,人工观察获知 funcDecl_GlobalCounter == k  是关注处
  // 2.  在下一行 加条件断点('funcDecl_GlobalCounter == k') ,  调试运行, 即到达关注处
  IdentifierInfo *ident = funcDecl->getIdentifier();
  if(ident!= nullptr){
    const StringRef &funcName = ident->getName();
    if(!funcName.empty()){
      std::string funcNameStr=funcName.str();
        std::string msg=fmt::format("函数名=[{}],[调试工具_函数声明_全局计数器]={},", funcName.str(),funcDecl_GlobalCounter);
        UtilPrintAstNode::printDecl(Ctx_ref, CI_ref, "调试",msg,funcDecl,true);
    }
  }else{
    std::string msg=fmt::format("函数名为空, [调试工具_函数声明_全局计数器]={},",  funcDecl_GlobalCounter);
    UtilPrintAstNode::printDecl(Ctx_ref, CI_ref, "调试",msg,funcDecl,true);
  }



}

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
