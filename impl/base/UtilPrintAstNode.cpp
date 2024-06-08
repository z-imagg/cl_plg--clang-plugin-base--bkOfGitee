// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "UtilPrintAstNode.h"
#include "base/UtilGetSrcTxtBySrcRange.h"
#include "base/UtilGetSrcFilePathByLoc.h"
#include "base/UtilParentKind.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include "base/Util.h"
#include "base/UtilPrintAstNode.h"

void UtilPrintAstNode::printStmt(ASTContext &Ctx, CompilerInstance &CI, std::string tag, std::string title, const Stmt *stmt,
                                 bool printSourceText) {
  //region title后面接上parent0的kind
  DynTypedNodeList parentS=Ctx.getParents(*stmt);
  size_t parentSSize=parentS.size();
  if(parentSSize>0){
    ASTNodeKind parent0NodeKind=parentS[0].getNodeKind();
    const std::string &parent0NodeKindStr = parent0NodeKind.asStringRef().str();
//    char msg[128];
    //sprintf中不要给 clang::StringRef类型，否则结果是怪异的。
//    sprintf(msg, ",parent0NodeKind:%s", parent0NodeKindCStr);
//    title.append(",parent0NodeKind:");
//    title.append(parent0NodeKindCStr);//这里用 parent0NodeKind.asStringRef().str().c_str() 结果也可能是怪异的
    title=fmt::format("{},parent0NodeKind:{}",title,parent0NodeKindStr);
  }
  //endregion

  int64_t stmtID = stmt->getID(Ctx);
  SourceManager & SM=CI.getSourceManager();
  const char *stmtClassName = stmt->getStmtClassName();
  Stmt::StmtClass stmtClass = stmt->getStmtClass();
  FileID fileId = SM.getFileID(stmt->getBeginLoc());
  SourceRange sourceRange=stmt->getSourceRange();

  printSourceRange(stmtID,
                   CI,
                   tag, title,
                   fileId, sourceRange,
                   "getStmtClassName", stmtClassName,
                   "getStmtClass", stmtClass,
                   NULL,EMPTY_ENUM_VAL,
                   NULL,EMPTY_ENUM_VAL,
                   printSourceText);

}

void UtilPrintAstNode::printExpr(ASTContext &Ctx, CompilerInstance &CI, std::string tag, std::string title, Expr *expr,
                                 bool printSourceText) {
  int64_t exprID = expr->getID(Ctx);
  SourceManager & SM=CI.getSourceManager();
  const char *stmtClassName = expr->getStmtClassName();
  Stmt::StmtClass stmtClass = expr->getStmtClass();
  ExprValueKind valueKind = expr->getValueKind();
  ExprObjectKind objectKind = expr->getObjectKind();
  FileID fileId = SM.getFileID(expr->getBeginLoc());
  SourceRange sourceRange=expr->getSourceRange();

  printSourceRange(exprID,
                   CI,
                   tag, title,
                   fileId, sourceRange,
                   "getStmtClassName", stmtClassName,
                   "getStmtClass", stmtClass,
                   "getValueKind", valueKind,
                   "getObjectKind", objectKind,
                   printSourceText);

}

void UtilPrintAstNode::printDecl(ASTContext &Ctx, CompilerInstance &CI, std::string tag, std::string title, Decl *decl,
                                 bool printSourceText) {
  int64_t declID = decl->getID();
  unsigned int declGlobalID = decl->getGlobalID();
  SourceManager & SM=CI.getSourceManager();
  const char *kindName = decl->getDeclKindName();
  Decl::Kind kind = decl->getKind();
  FileID fileId = SM.getFileID(decl->getBeginLoc());
  SourceRange sourceRange=decl->getSourceRange();
  printSourceRange(declID,
                   CI,
                   tag,title,
                   fileId,sourceRange,
                   "getDeclKindName",kindName,
                   "getKind",kind,
                   NULL,EMPTY_ENUM_VAL,
                   NULL,EMPTY_ENUM_VAL,
                   printSourceText);

}

void  UtilPrintAstNode::printSourceRange(int64_t nodeID,
                                         CompilerInstance& CI,
                                         std::string tag, std::string title,
                                         FileID fileId, const SourceRange &sourceRange,
                                         const char *topCategoryFieldName, const char *topCategoryName,
                                         const char *topCategoryEnumFieldName, int topCategoryEnum,
                                         const char *category1FieldName, int category1Enum,
                                         const char *category2FieldName, int category2Enum,
                                         bool printSourceText, int printSourceTextLimit){
  SourceManager & SM=CI.getSourceManager();
  FileID mainFileId = SM.getMainFileID();
//  FileID fileId = SM.getFileID(caseKSrcRange.getBegin());

  const std::tuple<std::string, std::string> & frst = UtilGetSrcTxtBySrcRange::get_FileAndRange_SourceText(sourceRange,CI);
  std::string fileAndRange=std::get<0>(frst);
  std::string sourceText=std::get<1>(frst);


  std::cout
    << tag << ","
    << title
    << ",nodeID:" << nodeID
    << ",文件路径、坐标:" << fileAndRange
    << "," << topCategoryFieldName << ":" << topCategoryName
    << "," << topCategoryEnumFieldName << ":" << topCategoryEnum
    <<  ",mainFileId:" << mainFileId.getHashValue()
    << ",fileId:" << fileId.getHashValue()
    ;
  if(category1Enum!=EMPTY_ENUM_VAL){
  std::cout
    << "," << category1FieldName << ":" << category1Enum
    ;
  }
  if(category2Enum!=EMPTY_ENUM_VAL){
    std::cout
      << "," << category2FieldName << ":" << category2Enum
    ;
  }
  if(printSourceText && sourceText.size()<printSourceTextLimit){
    std::cout <<   ",源码:【" << sourceText << "】" ;
  }

  std::cout  << std::endl;

}

void  UtilPrintAstNode::printSourceRangeSimple(
                             CompilerInstance& CI,
                             std::string tag, std::string title,
                              const SourceRange &sourceRange,
                             bool printSourceText,int printSourceTextLimit){

  SourceManager & SM=CI.getSourceManager();
  FileID fileId = SM.getFileID(sourceRange.getBegin() );
  FileID mainFileId = SM.getMainFileID();

  const std::tuple<std::string, std::string> & frst = UtilGetSrcTxtBySrcRange::get_FileAndRange_SourceText(sourceRange,CI);
  std::string fileAndRange=std::get<0>(frst);
  std::string sourceText=std::get<1>(frst);


  std::cout
          << tag << ","
          << title
          << ",文件路径、坐标:" << fileAndRange
          <<  ",mainFileId:" << mainFileId.getHashValue()
          << ",fileId:" << fileId.getHashValue()
          ;
  if(printSourceText && sourceText.size()<printSourceTextLimit){
    std::cout <<   ",源码:【" << sourceText << "】" ;
  }
  std::cout  << std::endl;

}