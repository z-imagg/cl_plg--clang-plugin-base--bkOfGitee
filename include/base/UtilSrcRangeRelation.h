// Created by z on 2024/6/8.

#ifndef UtilSrcRangeRelation_H
#define UtilSrcRangeRelation_H


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

class UtilSrcRangeRelation {

public:
//父亲节点们 转为 指定类型 的 节点列表, 注意模板方法只能放在头文件中.
//位置范围A 是否 包含 位置范围B ， 相等也算包含。
 // 由于SourceRange::fullyContains的结果是错误的，因此用自制同名方法替代
 static bool fullContains(SourceManager& SM, SourceRange A, SourceRange B);
};


#endif //UtilSrcRangeRelation_H
