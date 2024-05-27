#include "base/ASTContextUtil.h"


//  Ctx.langOpts.CPlusPlus 估计只能表示当前是用clang++编译的、还是clang编译的, [TODO] 但不能涵盖 'extern c'、'extern c++'造成的语言变化
bool  ASTContextUtil::useCxx(ASTContext *Ctx){
  const LangOptions &langOpts = Ctx->getLangOpts();
  bool _useCXX = (langOpts.CPlusPlus == 1);
  return _useCXX;
}