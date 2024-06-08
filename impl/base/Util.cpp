
#include "base/Util.h"

#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Frontend/CompilerInstance.h>
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "base/UtilParentKind.h"
#include "base/UtilGetSrcFilePathByLoc.h"
#include "base/UtilGetSrcTxtBySrcRange.h"
#include "base/UtilPrintAstNode.h"
#include "base/UtilInsertInclude.h"
#include "base/UtilEndStmtOf.h"
#include "base/UtilFuncIsX.h"
#include "base/UtilRewriteBuffer.h"
#include "base/UtilCompoundStmt.h"
#include "base/UtilLineNum.h"
#include "base/UtilMainFile.h"
#include "base/UtilLocId.h"
#include "base/UtilIsSysSrcFileOrMe.h"
#include "base/UtilStmtEndSemicolon.h"
#include "base/UtilFile.h"
#include "base/UtilEnvVar.h"
#include "base/UtilFuncIsX.h"
#include "base/UtilInsertInclude.h"
#include "base/UtilFuncDecl.h"
#include "base/UtilDiagnostics.h"
#include "base/UtilSubStmt.h"
#include "base/UtilConvertNodeType.h"
#include "base/UtilNextToken.h"
#include "base/UtilSrcRangeRelation.h"
#include "base/UtilRetStmt.h"
#include "base/UtilAttrKind.h"
#include "base/UtilStmtLs.h"
#include "base/UtilEditBuffer.h"
#include "base/UtilStr.h"
#include "base/UtilGetSrcTxtBySrcRange.h"
#include "UtilVarDecl.h"
#include <clang/AST/ParentMapContext.h>

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>


using namespace llvm;
using namespace clang;


//region 函数Util::collectParentS 的使用例子.
void __collectParentS__call_demo(SourceManager& SM, ASTContext &ctx,clang::Stmt *stmt) {
  std::vector<std::tuple<ASTNodeKind,SourceRange,const Stmt*>>  parentVec;
  const DynTypedNodeList & parents = ctx.getParents(*stmt);
  UtilConvertNodeType::collectParentS<Stmt>(parents,parentVec);
  //返回父亲节点们放在 向量parentVec 中
  size_t parentSize=parentVec.size();
  //共parentSize个父亲节点
  if(parentSize>1){
    for(int i =0; i < parentSize; i++){
//      parentINodeKind表示第i个父亲节点的ASTNodeKind
      ASTNodeKind parentINodeKind=std::get<0>(parentVec[i]);
//      parentISourceRange表示第i个父亲节点的位置范围
      SourceRange parentISourceRange=std::get<1>(parentVec[i]);
//      parentI表示第i个父亲节点
      const Stmt* parentI=std::get<2>(parentVec[i]);
      if(parentI==NULL){
        const std::string &parentISourceRangeText = parentISourceRange.printToString(SM);
        std::string parentINodeKindStr=parentINodeKind.asStringRef().str();
        std::string errMsg=fmt::format("第{}个父亲节点 ,位置范围 {},  并不是 Stmt类型，其ASTNodeKind是{}, 解决:把类型Stmt改为{}?\n",i,parentISourceRangeText,parentINodeKindStr,parentINodeKindStr);
        std::cout<<errMsg;
      }
    }
  }
}
//endregion

bool Util::LocIsInMacro(SourceLocation Loc, SourceManager& SM){
  bool isInMacro=
          SM.isAtStartOfImmediateMacroExpansion(Loc) || SM.isAtEndOfImmediateMacroExpansion(Loc) ||
          SM.isInSystemMacro(Loc) || SM.isMacroBodyExpansion(Loc) || SM.isMacroArgExpansion(Loc)
  ;
  return isInMacro;
}

//是否 独立且容器 语句
bool Util::isAloneContainerStmt(const Stmt *stmt){
  bool IsCompoundStmt=isa<CompoundStmt>(*stmt);
  bool IsIfStmt=isa<IfStmt>(*stmt);
  bool IsForStmt=isa<ForStmt>(*stmt);
  bool IsCXXForRangeStmt=isa<CXXForRangeStmt>(*stmt);
  bool IsWhileStmt=isa<WhileStmt>(*stmt);
  bool IsDoStmt=isa<DoStmt>(*stmt);
  bool IsSwitchStmt=isa<SwitchStmt>(*stmt);
  bool isContainerStmt = IsCompoundStmt || IsIfStmt || IsForStmt || IsCXXForRangeStmt || IsWhileStmt || IsDoStmt || IsSwitchStmt;
  return isContainerStmt;
}

std::string Util::pointerToString(void* ptr) {
  return std::to_string(reinterpret_cast<long long>(ptr));
}


