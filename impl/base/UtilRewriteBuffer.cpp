// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
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
#include "base/UtilRewriteBuffer.h"

std::string UtilRewriteBuffer::rewriteBufferToString(const RewriteBuffer &buffer) {
  return std::string(buffer.begin(), buffer.end());
}

void UtilRewriteBuffer::saveRewriteBuffer(const std::shared_ptr<Rewriter> rewriter_ptr, FileID mainFileId, std::string filePath) {
  const RewriteBuffer *pRewriteBuffer = rewriter_ptr->getRewriteBufferFor(mainFileId);
  UtilRewriteBuffer::saveRewriteBuffer0(pRewriteBuffer, filePath, "saveRewriteBuffer:");
}

void UtilRewriteBuffer::saveRewriteBuffer0(const RewriteBuffer *pRewriteBuffer, std::string filePath, std::string title){
//  const RewriteBuffer *pRewriteBuffer = rewriter_ptr->getRewriteBufferFor(mainFileId);
  std::string cppText = rewriteBufferToString(*pRewriteBuffer);

  std::ofstream fWriter;
  fWriter.open(filePath);
  fWriter << cppText ;
  fWriter.close();

  std::cout << title << filePath <<std::endl;
}