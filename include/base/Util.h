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

  //给定位置是否在宏中
    static bool LocIsInMacro(SourceLocation Loc, SourceManager& SM);

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

  static void emptyStrIfNullStr(const char* &cstr);
    /** c++11 手工实现 string_format
     * 模板，必须要放在头文件中.
     * @tparam Args
     * @param format
     * @param args
     * @return
     */
    


    static std::string pointerToString(void* ptr);

  static void saveEditBuffer(const std::shared_ptr<Rewriter> rewriter_ptr, FileID mainFileId, std::string filePath);


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


};


#endif //Util_H
