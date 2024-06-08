// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "UtilAttrKind.h"
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
#include "base/UtilAttrKind.h"

bool UtilAttrKind::hasAttrKind(Stmt *stmt, attr::Kind attrKind){
  if(!stmt){
    return false;
  }
//  clang::AttributedStmt* attributedStmt = clang::dyn_cast<clang::AttributedStmt>(stmt);

  AttributedStmt* attributedStmt = dyn_cast_or_null<AttributedStmt>(stmt);

//  AttributedStmt* attributedStmt= static_cast<AttributedStmt*> (stmt);//不能这样转，static_cast会把一个不是AttributedStmt的东西强硬转为AttributedStmt, 比如把一段不可写的代码区域转为Attr对象, 显然导致Segmentation fault而崩溃退出.

  if(attributedStmt){
    const ArrayRef<const Attr *> &attrS = attributedStmt->getAttrs();
    std::vector<const Attr *> attrVec(attrS.begin(), attrS.end());//方便调试看数组内容
    for(const Attr * attrJ:attrVec){
      if(!attrJ){
        continue;
      }
      attr::Kind attrJKind = attrJ->getKind();
      const std::string &normalizedFullName = attrJ->getNormalizedFullName();
//      std::cout << "AttributedStmt:" << attrJKind << "," << normalizedFullName << std::endl;
//    AttributedStmt:24,gnu::fallthrough
      if(attrKind==attrJKind){
        return true;
      }
    }//for结束

  }//if结束

  return false;
}