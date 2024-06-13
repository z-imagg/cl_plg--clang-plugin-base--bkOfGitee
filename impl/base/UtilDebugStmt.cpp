// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "base/UtilPrintAstNode.h"
#include "base/UtilGetSrcTxtBySrcRange.h"
#include "base/UtilDebugStmt.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/Stmt.h"
#include <clang/Frontend/CompilerInstance.h>

int UtilDebugStmt::stmt_GlobalCounter=0;
void UtilDebugStmt::debugStmtByGlobalCounter(      Stmt *stmt, ASTContext &Ctx_ref, CompilerInstance &CI_ref) {
  stmt_GlobalCounter++;
  // 调试步骤(语句全局计数器条件断点):
  // 1.  非调试地运行一次,人工观察获知 stmt_GlobalCounter == k  是关注处
  // 2.  在下一行 加条件断点('stmt_GlobalCounter == k') ,  调试运行, 即到达关注处
  const std::tuple<std::string, std::string> & frst = UtilGetSrcTxtBySrcRange::get_FileAndRange_SourceText(stmt->getSourceRange(),CI_ref);
  std::string fileAndRange=std::get<0>(frst);
  std::string sourceText=std::get<1>(frst);

  if(!sourceText.empty()){
        std::string msg=fmt::format("语句代码文本=[{}],[调试工具_函数声明_全局计数器]={},", sourceText,stmt_GlobalCounter);
        UtilPrintAstNode::printStmt(Ctx_ref, CI_ref, "调试",msg,stmt,true);
  }else{
    std::string msg=fmt::format("语句代码文本为空, [调试工具_函数声明_全局计数器]={},",  stmt_GlobalCounter);
    UtilPrintAstNode::printStmt(Ctx_ref, CI_ref, "调试",msg,stmt,true);
  }



}

bool UtilDebugStmt::ifCodeTxtEqPrintSrcTxt(      Stmt *stmt,std::string targetStmtTxt, ASTContext &Ctx_ref, CompilerInstance &CI_ref) {
  const std::tuple<std::string, std::string> & frst = UtilGetSrcTxtBySrcRange::get_FileAndRange_SourceText(stmt->getSourceRange(),CI_ref);
  std::string fileAndRange=std::get<0>(frst);
  std::string sourceText=std::get<1>(frst);

  bool equal=false;
    if(!sourceText.empty()){
      equal=(sourceText==targetStmtTxt);
      if(equal){
        std::string msg=fmt::format("找到语句=[{}],",sourceText);
        UtilPrintAstNode::printStmt(Ctx_ref, CI_ref, "调试",msg,stmt,true);
      }
    }

  return equal;


}
