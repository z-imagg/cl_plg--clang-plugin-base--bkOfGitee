// [术语] clang全局对象 == 'clang global object' == 'Cl G O' == ClGO

#ifndef ClGlbObj_H
#define ClGlbObj_H

#include <clang/Frontend/FrontendActions.h>

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/ArgumentsAdjusters.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "base/ActMain.h"

using namespace llvm;
using namespace clang;

class ClGO {
public:
    CompilerInstance &CI;
    SourceManager &SM;
    LangOptions &langOptions;
    Preprocessor &PP;
    ASTContext &astCtx;

public:
  ClGO(CompilerInstance &_CI,
       SourceManager &_SM,
       LangOptions &_langOptions,
       Preprocessor &_PP,
       ASTContext &_astCtx)
  :
  CI(_CI),
  SM(_SM),
  langOptions(_langOptions),
  PP(_PP),
  astCtx(_astCtx)
  {

  }

};


#endif //ClGlbObj_H
