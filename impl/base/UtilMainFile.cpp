// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "UtilMainFile.h"
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
#include "base/UtilMainFile.h"

Decl* UtilMainFile::firstDeclInMainFile(SourceManager&SM, std::vector<Decl*> declVec){
  for(int k=0; k < declVec.size(); k++){
    Decl* decl=declVec[k];
    SourceLocation Loc=decl->getBeginLoc();
    bool inMainFile=SM.isWrittenInMainFile(Loc);
    if(inMainFile){
      return decl;
    }
  }
  return NULL;
}

bool UtilMainFile::isDeclInMainFile(SourceManager&SM, Decl* D){
  //判断当前文件是否主文件
  bool inMainFile=SM.isWrittenInMainFile(D->getBeginLoc());
  return inMainFile;
}

void UtilMainFile::getMainFileIDMainFilePath(SourceManager& SM, FileID& mainFileId, std::string& mainFilePath){
  mainFileId = SM.getMainFileID();
  mainFilePath=SM.getFileEntryForID(mainFileId)->getName().str();
  return  ;
}

bool UtilMainFile::LocFileIDEqMainFileID(SourceManager& SM, SourceLocation Loc){
  FileID mainFileId = SM.getMainFileID();
  FileID fileId = SM.getFileID(Loc);
  bool LocInMainFile=(mainFileId==fileId);
//  bool LocInMainFile=SM.isWrittenInMainFile(Loc); //TODO 问 此方法LocFileIDEqMainFileID 可以被 SM.isWrittenInMainFile 替代吗?
  return LocInMainFile;
}