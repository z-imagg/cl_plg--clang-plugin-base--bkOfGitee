// Created by z on 2024/6/8.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <clang/AST/ParentMapContext.h>
#include "UtilFile.h"
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
#include "base/UtilFile.h"

void UtilFile::copySrcFile(std::string filePath, std::string destRootDir){
  //复制源文件 到 /tmp/, 方便开发查看. (适合cmake测试编译器，源文件用完即删除，导致此时出问题后拿不到源文件，难以复现问题）
  //  取当前时刻毫秒数
  std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  //  新文件路径、新文件目录构建、复制为新文件
//  std::string filePathCopy="/tmp/"+filePath+"_"+std::to_string(millis);
  std::string filePathCopy=destRootDir+"/"+filePath+"_"+std::to_string(millis);
  std::filesystem::path fpCopy(filePathCopy);
  const std::filesystem::path &dir = fpCopy.parent_path();
  std::filesystem::create_directories(dir);
  std::filesystem::copy(filePath, filePathCopy);
  std::cout << "查看，复制文件路径:" << filePath << "到,文件路径:" << filePathCopy << std::endl;

}