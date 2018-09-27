#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"

using namespace clang;

namespace {

    class ObjcSuperInitVisitor : public RecursiveASTVisitor<ObjcSuperInitVisitor>
    {
    public:
        bool VisitObjCInterfaceDecl(ObjCInterfaceDecl *declaration)
        {
            printf("ObjClass: %s\n", declaration->getNameAsString().c_str());
            return true;
        }

        bool VisitObjCImplementationDecl(ObjCImplementationDecl *declaration)
        {
            printf("Implementation: %s\n", declaration->getNameAsString().c_str());
            return true;
        }

        bool VisitObjCMethodDecl(ObjCMethodDecl *declaration)
        {
            if (!isInitImplementation(declaration)) {
                return true;
            }
            printf("Method: %s\n", declaration->getNameAsString().c_str());
            printf("Czy pozadany: %d\n", declaration->getCompoundBody() != nullptr);

            return true;
        }

    private:

        bool prefix(const std::string& a, const std::string& b) {
            if (a.size() > b.size()) {
                return a.substr(0,b.size()) == b;
            }
            else {
                return b.substr(0,a.size()) == a;
            }
        }

        bool isInitImplementation(ObjCMethodDecl *declaration) {
            return prefix(declaration->getNameAsString().c_str(), "init") && declaration->getCompoundBody();
        }

    };

    class ObjcSuperInitConsumer : public ASTConsumer
    {
    public:
        void HandleTranslationUnit(ASTContext &context) {
            visitor.TraverseDecl(context.getTranslationUnitDecl());
        }
    private:
        ObjcSuperInitVisitor visitor;
    };

    class ObjcSuperInitAction : public PluginASTAction
    {
    public:
        virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
            printf("Tworzymy\n");
            return llvm::make_unique<ObjcSuperInitConsumer>();
        }

        bool ParseArgs(const CompilerInstance &CI, const
                       std::vector<std::string>& args) {
            return true;
        }
    };

}

static clang::FrontendPluginRegistry::Add<ObjcSuperInitAction>
X("ObjcSuperInitPlugin", "Check that init call [super init] or [self init]");
