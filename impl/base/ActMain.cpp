#include "base/ActMain.h"

#include <clang/Frontend/FrontendActions.h>

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/ArgumentsAdjusters.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "Var/VarAstCnsm.h"

using namespace llvm;
using namespace clang;




int act_main(int Argc, const char **Argv,llvm::cl::OptionCategory optionCategory,const std::unique_ptr<clang::tooling::FrontendActionFactory> &frontendActionFactory,std::string actName,bool verbose ) {
  Expected<tooling::CommonOptionsParser> eOptParser =
          tooling::CommonOptionsParser::create(Argc, Argv, optionCategory);
  if (auto E = eOptParser.takeError()) {
    std::string err_msg(actName+"构建CommonOptionsParser出错");
    errs() << err_msg
           << toString(std::move(E)) << '\n';
    return EXIT_FAILURE;
  }

  // 获取编译配置
  clang::tooling::CompilationDatabase &Compilations = eOptParser->getCompilations();


  // 创建 CompilerInstance 对象
  clang::CompilerInstance CI;
  // 创建源管理器
  CI.createDiagnostics();
  CI.createFileManager();
  CI.createSourceManager(CI.getFileManager());
/* 确保在创建编译器实例（`clang::CompilerInstance`）后，立即创建源管理器（`clang::SourceManager`）并将其设置到编译器实例中
否则 ，运行时 断言失败 报错 ：   CompilerInstance.h:423: clang::SourceManager& clang::CompilerInstance::getSourceManager() const: Assertion `SourceMgr && "Compiler instance has no source manager!"' failed.
 */

  // 设置语言选项
  CI.getLangOpts().CPlusPlus = true;


  // 设置文件名
  const char* FileName = NULL;
  // 命令行 必须 指定源文件路径
  assert(Argc>=2 && Argv[1]);

  // 从 命令行 获取 源文件路径
  FileName=Argv[1];

  clang::FileID MainFileID = CI.getSourceManager().getOrCreateFileID(
          CI.getFileManager().getVirtualFile(FileName, /*Size=*/0, /*ModificationTime=*/0),
          clang::SrcMgr::C_User);

  // 将文件 ID 设置为主文件
  CI.getSourceManager().setMainFileID(MainFileID);

  // 创建 ClangTool 对象
  std::vector<std::string> Files{FileName};
  clang::tooling::ClangTool Tool(Compilations, Files);//报错： No member named 'getCompilations' in 'clang::CompilerInstance'


  CI.getDiagnosticOpts().ShowColors = true;

  if(verbose){
    Tool.appendArgumentsAdjuster( clang::tooling::getInsertArgumentAdjuster("--verbose"));
  }

  //解决： 找不到 stddef.h
  Tool.appendArgumentsAdjuster( clang::tooling::getInsertArgumentAdjuster({"-resource-dir","/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0"},tooling::ArgumentInsertPosition::END));
  /* stddef.h位于:
/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include/stddef.h
   */

//ActMain添加头文件路径例子
//  Tool.appendArgumentsAdjuster( clang::tooling::getInsertArgumentAdjuster({"-I","/pubx/build-llvm15/utils/TableGen"},tooling::ArgumentInsertPosition::END));
//  Tool.appendArgumentsAdjuster( clang::tooling::getInsertArgumentAdjuster({"-I","/pubx/llvm-project/llvm/utils/TableGen"},tooling::ArgumentInsertPosition::END));
//  Tool.appendArgumentsAdjuster( clang::tooling::getInsertArgumentAdjuster({"-I","/pubx/build-llvm15/include"},tooling::ArgumentInsertPosition::END));
//  Tool.appendArgumentsAdjuster( clang::tooling::getInsertArgumentAdjuster({"-I","/pubx/llvm-project/llvm/include"},tooling::ArgumentInsertPosition::END));

  // 运行 ClangTool
  int Result =   Tool.run(frontendActionFactory.get());

  return Result;
}

