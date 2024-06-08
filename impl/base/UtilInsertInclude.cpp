// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
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
#include "base/UtilInsertInclude.h"

void UtilInsertInclude::insertIncludeToFileStartByLoc(StringRef includeStmtText, SourceLocation Loc, SourceManager &SM, const std::shared_ptr<Rewriter> mRewriter_ptr){
  FileID fileId = SM.getFileID(Loc);

  bool insertResult;
  insertIncludeToFileStart(includeStmtText,fileId,SM,mRewriter_ptr,insertResult);
}

void UtilInsertInclude::insertIncludeToFileStart(StringRef includeStmtText, FileID fileId, SourceManager &SM, const std::shared_ptr<Rewriter> mRewriter_ptr, bool& insertResult)   {
//  SourceManager &SM = Context.getSourceManager();
//  FileID MainFileID = SM.getMainFileID();

//  FileID fileId = SM.getFileID(Loc);
  SourceLocation startLoc = SM.getLocForStartOfFile(fileId);

//我估计是 在第一次 Rewriter.InsertText 后 Rewriter.getRewriteBufferFor 才会有值, 即第一次插入时候 才建立的缓存
//   所以以下判断，不合适
//  const RewriteBuffer *RewriteBuf = mRewriter_ptr->getRewriteBufferFor(fileId);
//  if (!RewriteBuf){
//    return;
//  }


  insertResult=mRewriter_ptr->InsertText(startLoc, includeStmtText, true, true);
  return  ;
}