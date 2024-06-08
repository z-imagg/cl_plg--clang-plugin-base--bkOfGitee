
#include "base/Util.h"
#include "base/UtilSrcRange.h"

#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Frontend/CompilerInstance.h>
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
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
  Util::collectParentS<Stmt>(parents,parentVec);
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
  Util::extractLineAndColumn(SM,aB,aBL,aBC);
  SourceLocation aE = A.getEnd();
  int aEL,aEC;
  Util::extractLineAndColumn(SM,aE,aEL,aEC);

  SourceLocation xB = X.getBegin();
  int xBL,xBC;
  Util::extractLineAndColumn(SM,xB,xBL,xBC);
  SourceLocation xE = X.getEnd();
  int xEL,xEC;
  Util::extractLineAndColumn(SM,xE,xEL,xEC);

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

//从给定位置的Token移动到下一个Token所得的位置。 由于switch语句中冒号下一个Token位置的奇怪结果，导致此方法 是否在任何情况下都能实现 移动到下一个位置 有待确定
SourceLocation Util::nextTokenLocation(SourceLocation thisTokenLoc, const SourceManager& SM,const LangOptions& LO,int offset){
//    switch语句中冒号下一个Token位置的奇怪结果:
//    beginLoc = Lexer::getLocForEndOfToken(SwitchCase->getColonLoc(), /*Offset*/0, SM, LO);//offset:0 所取得位置是冒号右边 是期望位置
//    beginLoc = Lexer::getLocForEndOfToken(SwitchCase->getColonLoc(), /*Offset*/1, SM, LO);//offset:1 所取得位置是冒号左边 非期望位置
//按道理说 offset:1 冒号右边  ，offset:0 冒号左边 才对， 但结果是反过来的。不知道如何解释，据说是因为 冒号这个小Token 所在位置 同时 被一个更大的Token占据，导致offset:1 是 相对于 此大Token的，但这个解释有点勉强。
  //以下2步骤，确定能移动到下一个token位置？
  Token thisToken;
  //步骤1. 取给定位置的Token, 忽略空格、tab等空白符
  Lexer::getRawToken(thisTokenLoc, thisToken, SM, LO,/*IgnoreWhiteSpace:*/true);
  //步骤2. 相对 该Token的结束位置 右移1个Token 所获得的位置 即 下一个token位置
  const SourceLocation nextTokenLoc = Lexer::getLocForEndOfToken(thisToken.getEndLoc(), /*向右移1个Token*/offset, SM, LO);
  return nextTokenLoc;
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
//获取语句末尾分号位置
SourceLocation Util::getStmtEndSemicolonLocation(const Stmt *S, const SourceManager &SM,bool& endIsSemicolon) {
  const LangOptions &LO = LangOptions();
  Token JTok;

  // 获取Stmt的结束位置
  SourceLocation JLoc = S->getEndLoc();
  if(JLoc.isInvalid()){
    //如果语句末尾位置 就不合法，则标记没找到分号，再直接返回。
    endIsSemicolon= false;
    return JLoc;
  }

  do{

    Lexer::getRawToken(JLoc, JTok, SM, LO,/*IgnoreWhiteSpace:*/true);
    //忽略空白字符，IgnoreWhiteSpace：true，很关键，否则可能某个位置导致循环后还是该位置，从而死循环。
    JLoc = Lexer::getLocForEndOfToken(JTok.getEndLoc(), /*Offset*/1, SM, LO);
    //偏移量给1,Offset：1,很关键，如果不向前移动 可能循环一次还是在该位置，造成死循环。
    //取第J次循环的Token的结尾位置，JTok.getEndLoc()，很关键，否则可能下次循环还在该token上，导致死循环。
  }while (JTok.isNot(tok::semi)
          && JTok.isNot(tok::eof)
          && JTok.getLocation().isValid()
          );


  // 获取分号的结束位置

  endIsSemicolon=JTok.is(tok::semi);
  return JTok.getLocation();
}

//DiagnosticsEngine错误个数
std::string Util::strDiagnosticsEngineHasErr(DiagnosticsEngine &Diags){
//      DiagnosticsEngine &Diags = CI.getDiagnostics();
  int error=Diags.getNumErrors();
  bool hasErrorOccurred = Diags.hasErrorOccurred();
  bool hasFatalErrorOccurred = Diags.hasFatalErrorOccurred();
  bool hasUncompilableErrorOccurred = Diags.hasUncompilableErrorOccurred();
  bool hasUnrecoverableErrorOccurred = Diags.hasUnrecoverableErrorOccurred();
  std::string msg(fmt::format("DiagnosticsEngine错误个数: error:{},hasErrorOccurred:{},hasFatalErrorOccurred:{},hasUncompilableErrorOccurred:{},hasUnrecoverableErrorOccurred:{}",error,hasErrorOccurred,hasFatalErrorOccurred,hasUncompilableErrorOccurred,hasUnrecoverableErrorOccurred));
  return msg;
}
Decl* Util::firstDeclInMainFile(SourceManager&SM, std::vector<Decl*> declVec){
  for(int k=0; k < declVec.size(); k++){
    Decl* decl=declVec[k];
    SourceLocation Loc=decl->getBeginLoc();
    bool inMainFile=SM.isWrittenInMainFile(Loc);
    if(inMainFile){
      return decl;
    }
  }
  return NULL;
}

bool Util::isDeclInMainFile(SourceManager&SM, Decl* D){
  //判断当前文件是否主文件
  bool inMainFile=SM.isWrittenInMainFile(D->getBeginLoc());
  return inMainFile;
}

bool Util::LocIdSetContains(std::unordered_set<LocId,LocId>& _set, LocId locId){
  return !Util::LocIdSetNotContains(_set,locId);
}
bool Util::LocIdSetNotContains(std::unordered_set<LocId,LocId>& _set, LocId locId){
  bool contains=(_set.count(locId) <= 0);
  return contains;
}
void Util::getMainFileIDMainFilePath(SourceManager& SM,FileID& mainFileId,std::string& mainFilePath){
  mainFileId = SM.getMainFileID();
  mainFilePath=SM.getFileEntryForID(mainFileId)->getName().str();
  return  ;
}
int Util::childrenCntOfCompoundStmt(CompoundStmt* stmt){
  if(!stmt){
    return 0;
  }
//  int cnt=std::distance(stmt->child_begin(),stmt->child_end());
  return stmt->size();
}

/** void函数、构造函数 最后一条语句是return吗？
 * @param funcDesc
 * @return
 */
bool Util::isVoidFuncOrConstructorThenNoEndReturn(QualType funcReturnType, bool isaCXXConstructorDecl,Stmt *endStmtOfFuncBody){
  //void函数、构造函数 最后一条语句若不是return，则需在最后一条语句之后插入  函数释放语句
  if(funcReturnType->isVoidType() || isaCXXConstructorDecl){
    //是void函数 或是 构造函数: 此两者都可以末尾不显示写出return语句
//    Stmt *endStmtOfFuncBody = funcDesc.endStmtOfFuncBody;
    bool endStmtIsReturn=Util::isReturnStmtClass(endStmtOfFuncBody);
    if(!endStmtIsReturn){
      return true;
    }
  }
  return false;
}

bool Util::GetCompoundLRBracLoc(CompoundStmt*& compoundStmt, SourceLocation& funcBodyLBraceLoc, SourceLocation& funcBodyRBraceLoc){
  if( compoundStmt ){
    funcBodyLBraceLoc = compoundStmt->getLBracLoc();
    funcBodyRBraceLoc = compoundStmt->getRBracLoc();
    return true;
  }
  return false;
}
/**
 *
 * @param funcBody
 * @param funcBodyLBraceLoc
 * @return 若是组合语句(CompoundStmt) ，则取左花括号位置
 */
bool Util::funcBodyIsCompoundThenGetLRBracLoc(Stmt *funcBody, CompoundStmt*& compoundStmt, SourceLocation& funcBodyLBraceLoc, SourceLocation& funcBodyRBraceLoc){
  if( compoundStmt = dyn_cast<CompoundStmt>(funcBody)){
    funcBodyLBraceLoc = compoundStmt->getLBracLoc();
    funcBodyRBraceLoc = compoundStmt->getRBracLoc();
    return true;
  }
  return false;
}
bool Util::funcIsDefault(FunctionDecl *funcDecl){
  bool isDefault=funcDecl->isExplicitlyDefaulted() || funcDecl->isDefaulted();
  return isDefault;
}
bool Util::funcIsInline(FunctionDecl *funcDecl){

    bool isInline=
            funcDecl->isInlined()
            || funcDecl->isInlineSpecified()
            ;

    //TODO 以下两个是否能作为inline的一种，不确定
//  funcDecl->isInlineBuiltinDeclaration();
//  funcDecl->isInlineNamespace();

    return isInline;
}
bool Util::cxxConstructorIsDefault(CXXConstructorDecl *cxxCnstrDecl){
  bool isDefault= cxxCnstrDecl->isExplicitlyDefaulted() || cxxCnstrDecl->isDefaulted() || cxxCnstrDecl->isDefaultConstructor();
  return isDefault;
}

void Util::emptyStrIfNullStr(const char* &cstr){
//  whoInserted=(whoInserted==NULL?"":whoInserted);
  cstr=(cstr==NULL?"":cstr);
}

std::string Util::pointerToString(void* ptr) {
  return std::to_string(reinterpret_cast<long long>(ptr));
}
bool Util::isSysSrcFile(StringRef fn) {
  bool startWithUsr=fn.startswith("/usr/");
  bool isLLVM01=fn.startswith("/app/llvm_release_home/clang+llvm");
  bool isLLVM02=fn.startswith("/app/llvm_release_home/clang+llvm");
  bool isInternal=(startWithUsr||isLLVM01||isLLVM02);
  return isInternal;
}
bool Util::isRuntimeSrcFile(StringRef fn,std::string runtimeBaseName) {
  std::string headerF=runtimeBaseName.append(".h");
  std::string cF=runtimeBaseName.append(".c");
  std::string cppF=runtimeBaseName.append(".cpp");
  bool isTick =
          fn.endswith(headerF)
          || fn.endswith(cF)
          || fn.endswith(cppF)
  ;
  return isTick;
}

void Util::copySrcFile(std::string filePath,std::string destRootDir){
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
bool Util::LocFileIDEqMainFileID(SourceManager& SM, SourceLocation Loc){
  FileID mainFileId = SM.getMainFileID();
  FileID fileId = SM.getFileID(Loc);
  bool LocInMainFile=(mainFileId==fileId);
//  bool LocInMainFile=SM.isWrittenInMainFile(Loc); //TODO 问 此方法LocFileIDEqMainFileID 可以被 SM.isWrittenInMainFile 替代吗?
  return LocInMainFile;
}
bool Util::envVarEq(std::string varName, std::string varValueExpect){
  if(varName.empty()){
    return false;
  }
  const char* varValueReal=std::getenv(varName.c_str());
  if(varValueReal == NULL){
    return false;
  }
  bool eq= (varValueExpect == varValueReal);
  return eq;
}

void Util::saveEditBuffer(const std::shared_ptr<Rewriter> rewriter_ptr, FileID mainFileId, std::string filePath) {
  RewriteBuffer &editBuffer = rewriter_ptr->getEditBuffer(mainFileId);
  Util::saveRewriteBuffer0(&editBuffer,filePath,"saveEditBuffer:");
}

std::string Util::rewriteBufferToString(const RewriteBuffer &buffer) {
  return std::string(buffer.begin(), buffer.end());
}
void Util::saveRewriteBuffer(const std::shared_ptr<Rewriter> rewriter_ptr, FileID mainFileId, std::string filePath) {
  const RewriteBuffer *pRewriteBuffer = rewriter_ptr->getRewriteBufferFor(mainFileId);
  Util::saveRewriteBuffer0(pRewriteBuffer,filePath,"saveRewriteBuffer:");
}

void Util::saveRewriteBuffer0(const RewriteBuffer *pRewriteBuffer,std::string filePath,std::string title){
//  const RewriteBuffer *pRewriteBuffer = rewriter_ptr->getRewriteBufferFor(mainFileId);
  std::string cppText = rewriteBufferToString(*pRewriteBuffer);

  std::ofstream fWriter;
  fWriter.open(filePath);
  fWriter << cppText ;
  fWriter.close();

  std::cout << title << filePath <<std::endl;
}
bool Util::isLastCompoundStmt(CompoundStmt *stmt, ASTContext &context) {
  auto parents = context.getParents(*stmt);

  // 遍历父节点列表
  for (auto it = parents.begin(); it != parents.end(); ++it) {
    if (const FunctionDecl *func = it->get<FunctionDecl>()) {
      // 检查CompoundStmt是否为最后一个块
      Stmt *body = func->getBody();
      if (body && body == stmt) {
        return true;
      }
    } else if (const LambdaExpr *lambda = it->get<LambdaExpr>()) {
      // 检查CompoundStmt是否为lambda表达式的最后一个块
      Stmt *body = lambda->getBody();
      if(body){
        if (CompoundStmt *lambdaBody = dyn_cast<CompoundStmt>(body)) {
          Stmt *lastStmt = lambdaBody->body_back();
          if (lastStmt && lastStmt == stmt) {
            return true;
          }
        }
      }

    }
  }

  return false;
}
FunctionDecl *Util::getContainingFunction(CompoundStmt *stmt, ASTContext &context) {
  auto parents = context.getParents(*stmt);

  // 遍历父节点列表
  for (auto itJ = parents.begin(); itJ != parents.end(); ++itJ) {
    if (const LambdaExpr *lambdaJ = itJ->get<LambdaExpr>()) {
      // 返回包裹CompoundStmt的lambda
      CXXMethodDecl *methodJ = lambdaJ->getCallOperator();
      if (methodJ) {
        return methodJ;
      }
    } else if (const FunctionDecl *funcJ = itJ->get<FunctionDecl>()) {
      // 返回最近的FunctionDecl
      return const_cast<FunctionDecl*>(funcJ);
    }
  }

  return nullptr;
}



Stmt* Util::endStmtOfFunc(FunctionDecl *funcDecl) {
  Stmt *funcBody = funcDecl->getBody();
  return Util::endStmtOfCompoundStmt(funcBody);
}

Stmt* Util::endStmtOfCompoundStmt(Stmt *funcBody){
  if (funcBody && isa<CompoundStmt>(*funcBody)) {
    CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(funcBody);
    if (compoundStmt &&  (!compoundStmt->body_empty() ) ) {
      Stmt *lastStmt = compoundStmt->body_back();
      return lastStmt;
    }
  }
  return NULL;
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

std::vector<bool>  Util::subStmtIsFallThroughVec(const Stmt::child_range &subStmtLs ,Stmt* &negativeSecond,SourceManager& SM, LangOptions& langOptions) {
  std::vector<clang::Stmt*> subStmtVec(subStmtLs.begin(), subStmtLs.end());
  unsigned long subStmtCnt = subStmtVec.size();
  const std::vector<std::string> &textVec = Util::stmtLs2TextLs(subStmtVec, SM, langOptions);
  if(subStmtCnt>=2){
    //倒数第二条语句
    negativeSecond=subStmtVec[subStmtCnt-2];
  }

  //subStmtVec中的stmtJ是否为'[[gnu::fallthrough]];'
  std::vector<bool> subStmtIsFallThroughVec(subStmtCnt,false);

  for (std::size_t j = 0; j < subStmtCnt; ++j) {
    clang::Stmt* stmtJ = subStmtVec[j];
    subStmtIsFallThroughVec[j]=Util::hasAttrKind(stmtJ, attr::Kind::FallThrough);
      //如果本行语句是'[[gnu::fallthrough]];'  , 那么下一行前不要插入时钟语句, 否则语法错误.
  }
  return subStmtIsFallThroughVec;
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
void Util::extractLineAndColumn(const clang::SourceManager& SM, const clang::SourceLocation& sourceLocation, int& line, int& column) {
  clang::PresumedLoc presumedLoc = SM.getPresumedLoc(sourceLocation);
  line = presumedLoc.getLine();
  column = presumedLoc.getColumn();
  return;
}

bool Util::isEqSrcLocLineNum(const clang::SourceManager& SM, const clang::SourceLocation& srcLoc1, const clang::SourceLocation& srcLoc2) {
  clang::PresumedLoc presumedLoc1 = SM.getPresumedLoc(srcLoc1);
  clang::PresumedLoc presumedLoc2 = SM.getPresumedLoc(srcLoc2);
  int line1 = presumedLoc1.getLine();
  int line2 = presumedLoc2.getLine();
  bool lineNumEqual=(line1==line2);
  return lineNumEqual;
}
bool Util::parentIsCompound(ASTContext* astContext, const Stmt* currentStmt) {
  bool parentKindIsCompound= Util::parentKindIsSame(astContext, currentStmt, ASTNodeKind::getFromNodeKind<CompoundStmt>());
  bool parentClassIsCompound= Util::anyParentClassEqual(astContext, currentStmt, Stmt::CompoundStmtClass);
  bool parentIsCompound=parentKindIsCompound||parentClassIsCompound;
  return parentIsCompound;
}
//原名:parentClassEqual ，现名:anyParentClassEqual
bool Util::anyParentClassEqual(ASTContext* astContext, const Stmt* stmt, Stmt::StmtClass targetClass) {
  auto parents = astContext->getParents(*stmt);

  for (const auto& parent : parents) {
    auto stmtParent = parent.get<Stmt>();
    if (stmtParent && stmtParent->getStmtClass() == targetClass) {
      return true;
    }
  }

  return false;
}


bool Util::parentKindIsSame(ASTContext *Ctx, const Stmt* stmt, const ASTNodeKind& kind){
  if(!Ctx || !stmt){
    return false;
  }
  DynTypedNodeList parentS=Ctx->getParents(*stmt);
  for (const auto& parent : parentS) {
    if (   kind.isSame(parent.getNodeKind())  ) {
      return true;
    }
  }

  return false;
}
//若只有一个父亲，填充 该父亲节点的语句类型，返回true。否则 不填充，返回false。
bool Util::only1ParentNodeKind(CompilerInstance& CI, ASTContext& astContext, const Stmt* stmt , DynTypedNode& parent0, ASTNodeKind& parent0NK ) {
  SourceManager &SM = CI.getSourceManager();
  auto parents = astContext.getParents(*stmt);

  if(parents.size()!=1){
    return false;
  }
  /*DynTypedNode */parent0 = parents[0];
  parent0NK=parents[0].getNodeKind();

  return true;

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
void Util::insertIncludeToFileStartByLoc(StringRef includeStmtText,SourceLocation Loc, SourceManager &SM, const std::shared_ptr<Rewriter> mRewriter_ptr){
  FileID fileId = SM.getFileID(Loc);

  bool insertResult;
  insertIncludeToFileStart(includeStmtText,fileId,SM,mRewriter_ptr,insertResult);
}


void Util::insertCommentBeforeLoc(StringRef commentText,SourceLocation Loc , const std::shared_ptr<Rewriter> mRewriter_ptr,bool& insertResult)   {
  insertResult=mRewriter_ptr->InsertTextBefore(Loc, commentText);
  return  ;
}

void Util::insertIncludeToFileStart(StringRef includeStmtText,FileID fileId, SourceManager &SM, const std::shared_ptr<Rewriter> mRewriter_ptr,bool& insertResult)   {
//  SourceManager &SM = Context.getSourceManager();
//  FileID MainFileID = SM.getMainFileID();

//  FileID fileId = SM.getFileID(Loc);
  SourceLocation startLoc = SM.getLocForStartOfFile(fileId);

//我估计是 在第一次 Rewriter.InsertText 后 Rewriter.getRewriteBufferFor 才会有值, 即第一次插入时候 才建立的缓存
//   所以以下判断，不合适
//  const RewriteBuffer *RewriteBuf = mRewriter_ptr->getRewriteBufferFor(fileId);
//  if (!RewriteBuf){
//    return;
//  }


  insertResult=mRewriter_ptr->InsertText(startLoc, includeStmtText, true, true);
  return  ;
}

FunctionDecl* Util::findFuncDecByName(ASTContext *Ctx,std::string functionName){
//    std::string functionName = "calc";

  TranslationUnitDecl* translationUnitDecl=Ctx->getTranslationUnitDecl();
  for(auto decl:translationUnitDecl->decls()){
    if(FunctionDecl* funcDecl = dyn_cast<FunctionDecl>(decl)){
      if(funcDecl->getNameAsString()==functionName){
        return funcDecl;
      }
    }
  }
  return NULL;
}

std::vector<std::string> Util::stmtLs2TextLs(std::vector<Stmt*> stmtVec, SourceManager & SM, const LangOptions & langOptions){
  std::vector<std::string> textVec;

  std::transform(stmtVec.begin(), stmtVec.end(), std::back_inserter(textVec), [&SM,&langOptions](Stmt* stmt) {
      return UtilSrcRange::getSourceTextBySourceRange(stmt->getSourceRange(),SM,langOptions); // 这里可以根据需要进行转换操作
  });

  return textVec;
}
/**
 * 获取 给定 位置范围 的源码文本
 * @param sourceRange
 * @param sourceManager
 * @param langOptions
 * @return
 */
std::string Util::getSourceTextBySourceRange(SourceRange sourceRange, SourceManager & sourceManager, const LangOptions & langOptions){
  //ref:  https://stackoverflow.com/questions/40596195/pretty-print-statement-to-string-in-clang/40599057#40599057
//  SourceRange caseKSrcRange=S->getSourceRange();
  CharSourceRange charSourceRange=CharSourceRange::getCharRange(sourceRange);
  llvm::StringRef strRefSourceText=Lexer::getSourceText(charSourceRange, sourceManager, langOptions);

  std::string strSourceText=strRefSourceText.str();
  return strSourceText;
}

/**
 * 获取语句所属源文件路径
 */
void Util::getSourceFilePathOfStmt(const Stmt *S, const SourceManager &SM,StringRef& fn) {
  SourceLocation Loc = S->getBeginLoc();
  Util::getSourceFilePathAtLoc(Loc,SM,fn);
}

