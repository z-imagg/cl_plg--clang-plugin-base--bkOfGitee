
#ifndef UtilTraverseSingleParent_H
#define UtilTraverseSingleParent_H


#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTTypeTraits.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

// clang::DynTypedNode node=clang::DynTypedNode::create(*stmt);
class UtilTraverseSingleParent {
public:
  static bool traverseSingleParent(clang::ASTContext *Ctx_ref, const clang::Stmt* stmt_ptr );
};


#endif //UtilTraverseSingleParent_H
