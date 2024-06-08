
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
#include <clang/AST/ParentMapContext.h>

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>


using namespace llvm;
using namespace clang;


//开始位置、结束位置、插入者 转为 人类可读字符注释文本
void Util::BE_Loc_HumanText(SourceManager& SM, const SourceLocation beginLoc, const SourceLocation endLoc, const std::string whoInserted,std::string& humanTextComment){

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

bool Util::fullContains(SourceManager& SM, SourceRange A, SourceRange X){



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

void Util::emptyStrIfNullStr(const char* &cstr){
//  whoInserted=(whoInserted==NULL?"":whoInserted);
  cstr=(cstr==NULL?"":cstr);
}

std::string Util::pointerToString(void* ptr) {
  return std::to_string(reinterpret_cast<long long>(ptr));
}

void Util::saveEditBuffer(const std::shared_ptr<Rewriter> rewriter_ptr, FileID mainFileId, std::string filePath) {
  RewriteBuffer &editBuffer = rewriter_ptr->getEditBuffer(mainFileId);
  UtilRewriteBuffer::saveRewriteBuffer0(&editBuffer,filePath,"saveEditBuffer:");
}


bool Util::isReturnStmtClass(Stmt *stmt ){
  bool stmtIsReturn=false;
  if(stmt){
    Stmt::StmtClass endStmtClass = stmt->getStmtClass();
    if(Stmt::ReturnStmtClass==endStmtClass){
      stmtIsReturn=true;
    }
  }
  return stmtIsReturn;
}

bool Util::hasAttrKind(Stmt *stmt, attr::Kind attrKind){
  if(!stmt){
    return false;
  }
//  clang::AttributedStmt* attributedStmt = clang::dyn_cast<clang::AttributedStmt>(stmt);

  clang::AttributedStmt* attributedStmt = clang::dyn_cast_or_null<clang::AttributedStmt>(stmt);

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


/**取得声明语句中声明的变量个数
 * 在声明语句 中 声明的变量个数
 * 比如 :
 * 输入 "int i;",  返回1
 * 输入 "float sum,x=0,y;", 返回3
 * 输入 非声明语句,  返回0
 * 输入 非变量声明语句,  返回0
 * @param stmt
 * @return
 */
int Util::varCntInVarDecl(DeclStmt* declStmt) {
//  DeclStmt *declStmt = static_cast<DeclStmt *>(stmt);
  if(declStmt==NULL){
    return 0;
  }
//  if(declStmt){
    Decl *decl0 = *(declStmt->decl_begin());
    if(decl0 && decl0->getKind()==Decl::Kind::Var){
      //如果当前语句是声明语句, 且第一个子声明是变量声明语句,则栈变量分配个数填写1
      //  有可能是这种样子: int n,m,u,v=0;  应该取 declStmt->decls() 的size
      const DeclStmt::decl_range &declRange = declStmt->decls();
      // 取 declStmt->decls() 的size
      long declCnt = std::distance(declRange.begin(), declRange.end());
      return declCnt;
    }
    return 0;
//  }
}


std::vector<std::string> Util::stmtLs2TextLs(std::vector<Stmt*> stmtVec, SourceManager & SM, const LangOptions & langOptions){
  std::vector<std::string> textVec;

  std::transform(stmtVec.begin(), stmtVec.end(), std::back_inserter(textVec), [&SM,&langOptions](Stmt* stmt) {
      return UtilGetSrcTxtBySrcRange::getSourceTextBySourceRange(stmt->getSourceRange(),SM,langOptions); // 这里可以根据需要进行转换操作
  });

  return textVec;
}

