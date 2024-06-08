// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "UtilMacro.h"
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
#include "base/UtilMacro.h"

bool UtilMacro::LocIsInMacro(SourceLocation Loc, SourceManager& SM){
  bool isInMacro=
          SM.isAtStartOfImmediateMacroExpansion(Loc) || SM.isAtEndOfImmediateMacroExpansion(Loc) ||
          SM.isInSystemMacro(Loc) || SM.isMacroBodyExpansion(Loc) || SM.isMacroArgExpansion(Loc)
  ;
  return isInMacro;
}