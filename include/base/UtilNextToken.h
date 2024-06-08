// Created by z on 2024/6/8.

#ifndef UtilNextToken_H
#define UtilNextToken_H


#include <unordered_set>
#include "clang/Lex/Lexer.h"
#include "LocId.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include <sstream>
#include <clang/AST/ParentMapContext.h>
#include <clang/Frontend/CompilerInstance.h>
#include <set>
#include <clang/Rewrite/Core/Rewriter.h>
#include <string>

class UtilNextToken {

public:
//从给定位置的Token移动到下一个Token所得的位置。 由于switch语句中冒号下一个Token位置的奇怪结果，导致此方法 是否在任何情况下都能实现 移动到下一个位置 有待确定
static SourceLocation nextTokenLocation(SourceLocation thisTokenLocation, const SourceManager& SM,const LangOptions& LO,int offset=1);
};


#endif //UtilNextToken_H
