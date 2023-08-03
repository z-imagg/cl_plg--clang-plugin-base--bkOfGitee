#include "base/ActMain.h"

#include <clang/Frontend/FrontendActions.h>

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/ArgumentsAdjusters.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "Brc/BrcAstCnsm.h"

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

//  std::shared_ptr<DiagnosticOptions> diagnosticOptions = std::make_shared<DiagnosticOptions>();
//  std::shared_ptr<DiagnosticOptions> DiagOpts = std::make_shared<DiagnosticOptions>();
//  DiagnosticsEngine Diags(new DiagnosticIDs(), &*DiagOpts, new IgnoringDiagConsumer(), true);
//  std::shared_ptr<DiagnosticsEngine> Diags = std::make_shared<DiagnosticsEngine>( DiagOpts.get() );
//  DiagnosticOptions DiagOpts;
//  DiagnosticsEngine Diags(new DiagnosticIDs(), &DiagOpts, new IgnoringDiagConsumer(), true);

  DiagnosticOptions* DiagOpts=new DiagnosticOptions();
  DiagnosticsEngine *Diags=new DiagnosticsEngine(new DiagnosticIDs(), DiagOpts);


  // 创建 CompilerInstance 对象
//  std::shared_ptr<CompilerInstance> CI_PTR = std::make_shared<CompilerInstance>( );

  CompilerInstance *CI_PTR = new CompilerInstance( );

  clang::CompilerInstance & CI= *(CI_PTR );
  // 创建源管理器
//  CI.createDiagnostics(DiagOpts.get());
  CI.setDiagnostics(Diags);
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

  DiagnosticsEngine &de = CI.getDiagnostics();
  // 运行 ClangTool
  int Result =   Tool.run(frontendActionFactory.get());

  return Result;
}

