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
        bool VisitObjCMethodDecl(ObjCMethodDecl *declaration)
        {
            if (!isInitImplementation(declaration)) {
                return true;
            }
            printf("Method: [%s %s]\n", declaration->getClassInterface()->getNameAsString().c_str(), declaration->getNameAsString().c_str());
            bool callInit = containsInit(declaration->getCompoundBody());
            printf("Contains init: %d\n", callInit);

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

        bool isInit(std::string methodName) {
            return prefix(methodName, "init");
        }

        bool isInitImplementation(ObjCMethodDecl *declaration) {
            return isInit(declaration->getNameAsString()) && declaration->getCompoundBody();
        }

        bool containsInit(Stmt *statement) {
            if (statement->getStmtClass() == Stmt::ObjCMessageExprClass) {
                ObjCMessageExpr *objcExpr = (ObjCMessageExpr *)statement;
                if (isInit(objcExpr->getSelector().getAsString())) {
                    return true;
                }
            }

            for (auto child : statement->children()) {
                if (child && containsInit(child)) {
                    return  true;
                }
            }

            return false;
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
