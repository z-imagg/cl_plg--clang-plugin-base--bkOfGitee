// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "UtilFuncDecl.h"
#include "UtilInsertInclude.h"
#include "UtilFuncIsX.h"
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
#include "base/UtilFuncDecl.h"

FunctionDecl* UtilFuncDecl::findFuncDecByName(ASTContext *Ctx, std::string functionName){
//    std::string functionName = "calc";

  TranslationUnitDecl* translationUnitDecl=Ctx->getTranslationUnitDecl();
  for(auto decl:translationUnitDecl->decls()){
    if(FunctionDecl* funcDecl = dyn_cast<FunctionDecl>(decl)){
      if(funcDecl->getNameAsString()==functionName){
        return funcDecl;
      }
    }
  }
  return NULL;
}