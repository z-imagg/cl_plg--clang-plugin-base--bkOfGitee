//
// Created by zz on 2023/7/22.
//

#ifndef Util_H
#define Util_H


#include <string>

#include <clang/Rewrite/Core/Rewriter.h>
#include <set>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/AST/ParentMapContext.h>
#include <sstream>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "LocId.h"
#include "clang/Lex/Lexer.h"



#include <sstream>
#include <unordered_set>
// 放弃 c++11 string_format 宏实现、或std::sprintf实现


using namespace llvm;
using namespace clang;

class Util {
public:
    //开始位置、结束位置、插入者 转为 人类可读字符注释文本
    static void BE_Loc_HumanText(SourceManager& SM,const SourceLocation beginLoc,const SourceLocation endLoc,const std::string whoInserted,std::string& humanTextComment);

   //父亲节点们 转为 指定类型 的 节点列表, 注意模板方法只能放在头文件中.
/**
 * 父亲节点们 转为 指定类型 的 节点列表，
 *     指定类型 不合适， 即  若父亲i的类型  不是  指定类型，  则返回向量中i位置为NULL，
 *            可以通过返回向量中i位置的ASTNodeKind确定给定何类型合适。
 * @tparam ParentNodeTp 指定类型
 * @param parents 输入的 父亲节点们
 * @param parentVec 返回 元组<父亲i的ASTNodeKind,父亲i转为给定类型的结果> 向量
 */
    template<typename ParentNodeTp>
    static void collectParentS(const DynTypedNodeList &parents ,std::vector<std::tuple<ASTNodeKind,SourceRange,const ParentNodeTp*>> & parentVec){
//      const DynTypedNodeList &parents = ctx.getParents(*stmt);

      for (const DynTypedNode& parentNodeI : parents) {
        //注意  :  parentNodeI.get<T>():  如果parentNode不是类型T的，此get返回NULL
        const ParentNodeTp *parentI = parentNodeI.get<ParentNodeTp>();
        SourceRange parentISourceRange=parentNodeI.getSourceRange();

        const ASTNodeKind &parentINodeKind = parentNodeI.getNodeKind();
        parentVec.push_back(std::make_tuple(parentINodeKind, parentISourceRange,parentI));
      }
    }
    //位置范围A 是否 包含 位置范围B ， 相等也算包含。
    // 由于SourceRange::fullyContains的结果是错误的，因此用自制同名方法替代
    static bool fullContains(SourceManager& SM, SourceRange A, SourceRange B);
    //给定位置是否在宏中
    static bool LocIsInMacro(SourceLocation Loc, SourceManager& SM);
    //从给定位置的Token移动到下一个Token所得的位置。 由于switch语句中冒号下一个Token位置的奇怪结果，导致此方法 是否在任何情况下都能实现 移动到下一个位置 有待确定
    static SourceLocation nextTokenLocation(SourceLocation thisTokenLocation, const SourceManager& SM,const LangOptions& LO,int offset=1);
    /**是否 独立且容器 语句
     * 所谓 容器 即 能容纳别的语句 的语句
     * 所以 独立 即 能独立存在 而不会报语法错误 的语句
     *
     *注意 else 是容器语句，但else不能独立存在，因此 else 不是 独立且容器 语句
     *   else必须依附if才能存在
     * 块、if、for、while、do-while  是  独立且容器 语句
     * @param stmt
     * @return
     */
    static bool isAloneContainerStmt(const Stmt *stmt)  ;
    //获取语句末尾分号位置
    static SourceLocation getStmtEndSemicolonLocation(const Stmt *S, const SourceManager &SM,bool& endIsSemicolon)  ;
    
    //DiagnosticsEngine错误个数
    static std::string strDiagnosticsEngineHasErr(DiagnosticsEngine &Diags);
    //在给定声明列表中，找到第一个在MainFile的声明
    static Decl* firstDeclInMainFile(SourceManager&SM, std::vector<Decl*> declVec);
    //判断给定声明是否在主文件中
    static bool isDeclInMainFile(SourceManager&SM, Decl* D);

    static bool LocIdSetContains(std::unordered_set<LocId,LocId>& _set, LocId locId);
    static bool LocIdSetNotContains(std::unordered_set<LocId,LocId>& _set, LocId locId);
    static void getMainFileIDMainFilePath(SourceManager& SM,FileID& mainFileId,std::string& mainFilePath);
    static int childrenCntOfCompoundStmt(CompoundStmt* stmt);
    /** void函数、构造函数 最后一条语句是return吗？
     * @param funcDesc
     * @return
     */
    static bool isVoidFuncOrConstructorThenNoEndReturn(QualType funcReturnType, bool isaCXXConstructorDecl,Stmt *endStmtOfFuncBody);
    static bool GetCompoundLRBracLoc(CompoundStmt*& compoundStmt, SourceLocation& funcBodyLBraceLoc, SourceLocation& funcBodyRBraceLoc);
    /**
     *
     * @param funcBody
     * @param funcBodyLBraceLoc
     * @return 是否组合语句,即是否CompoundStmt
     */
    static bool funcBodyIsCompoundThenGetLRBracLoc(Stmt *funcBody, CompoundStmt*& compoundStmt, SourceLocation& funcBodyLBraceLoc, SourceLocation& funcBodyRBraceLoc);
    /**
     * default函数体举例:
     * 'void func1( ) = default;'
     * 'void func2( ) {}'
     * @param funcDecl
     * @return
     */
    static bool funcIsDefault(FunctionDecl *funcDecl);

    /**
     * inline函数举例:
     * TODO
     * @param funcDecl
     * @return
     */
    static bool funcIsInline(FunctionDecl *funcDecl);
    /**
     * default构造函数体举例:
     * 'User::User( ) = default;'
     * 'User::User( ) {}'
     * @param cxxCnstrDecl
     * @return
     */
    static bool cxxConstructorIsDefault(CXXConstructorDecl *cxxCnstrDecl);

    static void emptyStrIfNullStr(const char* &cstr);
    /** c++11 手工实现 string_format
     * 模板，必须要放在头文件中.
     * @tparam Args
     * @param format
     * @param args
     * @return
     */
    


    static std::string pointerToString(void* ptr);
    /**给定源文件路径是否系统源文件
 * 系统源文件路径举例：
/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/cpp_type_traits.h
/usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/ext/type_traits.h
/usr/include/x86_64-linux-gnu/bits/iscanonical.h

/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include/uintrintrin.h
 * @param fn
 * @return
 */
    static bool isSysSrcFile(StringRef fn);
    static bool isRuntimeSrcFile(StringRef fn,std::string runtimeBaseName) ;
    static void copySrcFile(std::string srcFilePath,std::string destRootDir="/tmp/");
    static bool LocFileIDEqMainFileID(SourceManager& SM, SourceLocation Loc);
    static void saveEditBuffer(const std::shared_ptr<Rewriter> rewriter_ptr, FileID mainFileId, std::string filePath);
    static std::string rewriteBufferToString(const RewriteBuffer &buffer);
    static void
    saveRewriteBuffer(const std::shared_ptr<Rewriter> rewriter_ptr, FileID mainFileId, std::string filePath);
    static bool envVarEq(std::string varName, std::string varValueExpect);

    static void saveRewriteBuffer0(const RewriteBuffer *pRewriteBuffer, std::string filePath, std::string title);
    static bool isLastCompoundStmt(CompoundStmt *stmt, ASTContext &context);
    static FunctionDecl *getContainingFunction(CompoundStmt *stmt, ASTContext &context);
    static  Stmt* endStmtOfFunc(FunctionDecl *funcDecl) ;
    static  Stmt* endStmtOfCompoundStmt(Stmt *funcBody) ;


    static bool isReturnStmtClass(Stmt *stmt );
    /**计算语句列表中各语句是否为FallThrough，附带返回倒数第二条语句
     *
     * @param subStmtLs
     * @param negativeSecond
     * @return
     */
    static std::vector<bool>  subStmtIsFallThroughVec(const Stmt::child_range &subStmtLs ,Stmt* &negativeSecond ,SourceManager& SM, LangOptions& langOptions);
    /**语句是否有该属性
     *
     * @param stmt
     * @param attrKind
     * @return
     */
    static bool hasAttrKind(Stmt *stmt, attr::Kind attrKind);
    static void extractLineAndColumn(const clang::SourceManager& SM, const clang::SourceLocation& sourceLocation, int& line, int& column);
    /** 两给定'SourceLocation'的行号是否相同
     *
     * @param SM
     * @param srcLoc1
     * @param srcLoc2
     * @return
     */
    static bool isEqSrcLocLineNum(const clang::SourceManager& SM, const clang::SourceLocation& srcLoc1, const clang::SourceLocation& srcLoc2);

  /**
   * 在声明语句 中 声明的变量个数
   * 比如 :
   * 输入 "int i;",  返回1
   * 输入 "float sum,x=0,y;", 返回3
   * 输入 非变量声明语句,  返回0
   * @param stmt
   * @return
   */
    static int varCntInVarDecl(DeclStmt* stmt);
    static void insertCommentBeforeLoc(StringRef commentText,SourceLocation Loc,   const std::shared_ptr<Rewriter> mRewriter_ptr,bool& insertResult);
    static void insertIncludeToFileStart(StringRef includeStmtText,FileID fileId, SourceManager &SM, const std::shared_ptr<Rewriter> rewriter_ptr,bool& insertResult);
    static void insertIncludeToFileStartByLoc(StringRef includeStmtText,SourceLocation Loc, SourceManager &SM, const std::shared_ptr<Rewriter> rewriter_ptr);

  static FunctionDecl* findFuncDecByName(ASTContext *Ctx,std::string functionName);
    static std::vector<std::string> stmtLs2TextLs(std::vector<Stmt*> stmtVec, SourceManager & SM, const LangOptions & langOptions);


};


#endif //Util_H
