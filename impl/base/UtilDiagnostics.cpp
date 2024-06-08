// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
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
#include "base/UtilDiagnostics.h"

//DiagnosticsEngine错误个数
std::string UtilDiagnostics::strDiagnosticsEngineHasErr(DiagnosticsEngine &Diags){
//      DiagnosticsEngine &Diags = CI.getDiagnostics();
  int error=Diags.getNumErrors();
  bool hasErrorOccurred = Diags.hasErrorOccurred();
  bool hasFatalErrorOccurred = Diags.hasFatalErrorOccurred();
  bool hasUncompilableErrorOccurred = Diags.hasUncompilableErrorOccurred();
  bool hasUnrecoverableErrorOccurred = Diags.hasUnrecoverableErrorOccurred();
  std::string msg(fmt::format("DiagnosticsEngine错误个数: error:{},hasErrorOccurred:{},hasFatalErrorOccurred:{},hasUncompilableErrorOccurred:{},hasUnrecoverableErrorOccurred:{}",error,hasErrorOccurred,hasFatalErrorOccurred,hasUncompilableErrorOccurred,hasUnrecoverableErrorOccurred));
  return msg;
}