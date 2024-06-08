// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "base/UtilVarDecl.h"
#include "base/UtilStr.h"
#include "base/UtilEditBuffer.h"
#include "base/UtilStmtLs.h"
#include "base/UtilAttrKind.h"
#include "base/UtilRetStmt.h"
#include "base/UtilSrcRangeRelation.h"
#include "base/UtilNextToken.h"
#include "base/UtilConvertNodeType.h"
#include "base/UtilSubStmt.h"
#include "base/UtilDiagnostics.h"
#include "base/UtilFuncDecl.h"
#include "base/UtilEnvVar.h"
#include "base/UtilFile.h"
#include "base/UtilStmtEndSemicolon.h"
#include "base/UtilIsSysSrcFileOrMe.h"
#include "base/UtilLocId.h"
#include "base/UtilMainFile.h"
#include "base/UtilLineNum.h"
#include "base/UtilCompoundStmt.h"
#include "base/UtilRewriteBuffer.h"
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
#include "base/UtilVarDecl.h"

/**取得声明语句中声明的变量个数
 * 在声明语句 中 声明的变量个数
 * 比如 :
 * 输入 "int i;",  返回1
 * 输入 "float sum,x=0,y;", 返回3
 * 输入 非声明语句,  返回0
 * 输入 非变量声明语句,  返回0
 * @param stmt
 * @return
 */
int UtilVarDecl::varCntInVarDecl(DeclStmt* declStmt) {
//  DeclStmt *declStmt = static_cast<DeclStmt *>(stmt);
  if(declStmt==NULL){
    return 0;
  }
//  if(declStmt){
    Decl *decl0 = *(declStmt->decl_begin());
    if(decl0 && decl0->getKind() == Decl::Var){
      //如果当前语句是声明语句, 且第一个子声明是变量声明语句,则栈变量分配个数填写1
      //  有可能是这种样子: int n,m,u,v=0;  应该取 declStmt->decls() 的size
      const DeclStmt::decl_range &declRange = declStmt->decls();
      // 取 declStmt->decls() 的size
      long declCnt = std::distance(declRange.begin(), declRange.end());
      return declCnt;
    }
    return 0;
//  }
}