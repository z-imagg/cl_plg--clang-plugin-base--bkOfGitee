
#ifndef ASTContextUtil_H
#define ASTContextUtil_H

#include <clang/AST/ASTContext.h>
#include <clang/Basic//LangOptions.h>

using namespace  clang;

class ASTContextUtil {
public:
  //  Ctx.langOpts.CPlusPlus 估计只能表示当前是用clang++编译的、还是clang编译的, [TODO] 但不能涵盖 'extern c'、'extern c++'造成的语言变化
  static bool  useCxx(ASTContext *Ctx);
};


#endif //ASTContextUtil_H
