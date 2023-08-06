
#include "base/LocId.h"

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <fmt/core.h>
#include "base/Util.h"

using namespace clang;


    LocId LocId::buildFor(std::string fp, const SourceLocation funcDeclBeginLoc, const clang::SourceManager& SM){
//      const SourceLocation &funcDeclBeginLoc = funcDecl->getBeginLoc();
      int line;
      int column;
//      Decl::Kind kind = funcDecl->getKind();
//      Stmt::StmtClass xx = funcDecl->getBody()->getStmtClass();
      Util::extractLineAndColumn(SM,funcDeclBeginLoc,line,column);
      return LocId(fp,line,column);
    }

    std::string LocId::to_string(){
      return fmt::format("{}:{},{}",filePath,line,column);
    }
LocId:: LocId(
//            Decl::Kind declKind, Stmt::StmtClass stmtClass,
            std::string filePath,int line, int column)
    :
//    declKind(declKind),
//    stmtClass(stmtClass),
      filePath(filePath),
      line(line),
    column(column)
    {

    }


    bool LocId::containedByRange(SourceManager&SM, SourceRange range){
      SourceLocation B=range.getBegin();
      SourceLocation E=range.getEnd();

      int bL=-1,bC=-1;
      Util::extractLineAndColumn(SM,B,bL,bC);
      int eL=-1,eC=-1;
      Util::extractLineAndColumn(SM,E,eL,eC);

      bool in_range=
          (bL<line || (bL==line && bC<column))
          &&
          (eL>line || (eL==line && eC>column))
        ;

      return in_range;
    }

    // 重写哈希函数
    size_t LocId::operator()(const LocId& that) const {
      // 同一个文件的 在同一个链条（hash）
      return std::hash<std::string>()(that.filePath) ;
    }

    // 重写相等操作符
    bool LocId::operator==(const LocId& that) const {
      return
//      this->stmtClass== that.stmtClass &&
      this->line== that.line
       && this->column== that.column
       && this->filePath== that.filePath
      ;
    }


