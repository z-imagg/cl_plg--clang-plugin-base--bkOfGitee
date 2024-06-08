// Created by z on 2024/6/8.

#include "base/UtilGetSrcTxtBySrcRange.h"
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
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "base/UtilGetSrcTxtBySrcRange.h"
#include "base/UtilGetSrcFilePathByLoc.h"
#include "base/UtilParentKind.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include "base/Util.h"
#include "base/UtilGetSrcTxtBySrcRange.h"

/**
 * 获取 给定 位置范围 的源码文本
 * @param sourceRange
 * @param sourceManager
 * @param langOptions
 * @return
 */
std::string UtilGetSrcTxtBySrcRange::getSourceTextBySourceRange(SourceRange sourceRange, SourceManager & sourceManager, const LangOptions & langOptions){
  //ref:  https://stackoverflow.com/questions/40596195/pretty-print-statement-to-string-in-clang/40599057#40599057
//  SourceRange caseKSrcRange=S->getSourceRange();
  CharSourceRange charSourceRange=CharSourceRange::getCharRange(sourceRange);
  StringRef strRefSourceText=Lexer::getSourceText(charSourceRange, sourceManager, langOptions);

  std::string strSourceText=strRefSourceText.str();
  return strSourceText;
}

/**开发用工具 get_FileAndRange_SourceText ： 获得SourceRange的 文件路径、文件中的坐标、源码文本
 *
 * @param sourceRange
 * @param CI
 * @return
 */

std::tuple<std::string,std::string>  UtilGetSrcTxtBySrcRange::get_FileAndRange_SourceText(const SourceRange &sourceRange, CompilerInstance& CI){
  //{开发用
  SourceManager &SM = CI.getSourceManager();
  LangOptions &langOpts = CI.getLangOpts();
//      const SourceRange &caseKSrcRange = Decl->getSourceRange();
  std::string fileAndRange = sourceRange.printToString(SM);
  std::string sourceText = UtilGetSrcTxtBySrcRange::getSourceTextBySourceRange(sourceRange, SM, langOpts);
  return std::tuple<std::string,std::string>(fileAndRange,sourceText);
  //}
}

//开始位置、结束位置、插入者 转为 人类可读字符注释文本
void UtilGetSrcTxtBySrcRange::BE_Loc_HumanText(SourceManager& SM, const SourceLocation beginLoc, const SourceLocation endLoc, const std::string whoInserted, std::string& humanTextComment){

  const PresumedLoc &BLocPr = SM.getPresumedLoc(beginLoc);
  std::string BLocPr_str = fmt::format(
    "valid:{},fileID:{},{}:{}:{}",
    BLocPr.isValid(),
    BLocPr.getFileID().getHashValue(),
    BLocPr.getFilename(),
    BLocPr.getLine(),
    BLocPr.getColumn()
  );

  const PresumedLoc &ELocPr = SM.getPresumedLoc(endLoc);

  //若果B、E 文件名相同，则省略E文件名，以节省占地长度，增加人类可阅读性。
  std::string BLocPrFN = BLocPr.getFilename();
  std::string ELocPrFN = ELocPr.getFilename();
  if (ELocPrFN==BLocPrFN){
    ELocPrFN="同始";
  }

  std::string ELocPr_str = fmt::format(
      "valid:{},fileID:{},{}:{}:{}",
      ELocPr.isValid(),
      ELocPr.getFileID().getHashValue(),
      ELocPrFN,
      ELocPr.getLine(),
      ELocPr.getColumn()
  );


  humanTextComment=fmt::format("/*{}，始【{}】，终【{}】*/", whoInserted, BLocPr_str, ELocPr_str);

}