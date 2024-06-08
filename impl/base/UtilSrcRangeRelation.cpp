// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
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
#include "base/UtilSrcRangeRelation.h"

bool UtilSrcRangeRelation::fullContains(SourceManager& SM, SourceRange A, SourceRange X){



  SourceLocation aB = A.getBegin();
  int aBL,aBC;
  UtilLineNum::extractLineAndColumn(SM,aB,aBL,aBC);
  SourceLocation aE = A.getEnd();
  int aEL,aEC;
  UtilLineNum::extractLineAndColumn(SM,aE,aEL,aEC);

  SourceLocation xB = X.getBegin();
  int xBL,xBC;
  UtilLineNum::extractLineAndColumn(SM,xB,xBL,xBC);
  SourceLocation xE = X.getEnd();
  int xEL,xEC;
  UtilLineNum::extractLineAndColumn(SM,xE,xEL,xEC);

  bool _fullContains=
  (aBL < xBL || (aBL == xBL && aBC <= xBC) )    &&  //A开头位置行号 小于 x开头位置行号 , 行号相等时 列号同理
  (aEL > xEL || (aEL == xEL && aEC >= xEC) )  //且    A末尾位置行号 大于 x末尾位置行号 , 行号相等时 列号同理
  ;                                           //即 表示 A范围 包含了 B范围

  //由此可见 范围A 和 范围X 等同时，此方法也返回true

  return _fullContains;
}