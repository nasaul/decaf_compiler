/* File: ast_decl.h
 * ----------------
 * In our parse tree, Decl nodes are used to represent and
 * manage declarations. There are 4 subclasses of the base class,
 * specialized for declarations of variables, functions, classes,
 * and interfaces.
 *
 * pp3: You will need to extend the Decl classes to implement
 * semantic processing including detection of declaration conflicts
 * and managing scoping issues.
 */

#ifndef _H_ast_decl
#define _H_ast_decl

#include "ast.h"
#include "ast_stmt.h"
#include "ast_type.h"
#include "list.h"

class Type;
class NamedType;
class Identifier;
class Stmt;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/// Decl
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class Decl : public Node
{
  protected:
    Identifier *id;
    Scope *scope;

  public:
    Decl(Identifier *name);
    friend std::ostream& operator<<(std::ostream& out, Decl *d) { return out << d->id; }

    virtual bool AreEquiv(Decl *other);

    const char* Name() { return id->Name(); }
    Scope* GetScope() { return scope; }

    virtual void ScopeMake(Scope *parent);
    virtual void Check() = 0;
    Identifier *GetId() { return id; }
    const char *GetName() { return id->GetName(); }

    virtual bool ConflictsWithPrevious(Decl *prev);

    virtual bool IsVarDecl() { return false; } // jdz: could use typeid/dynamic_cast for these
    virtual bool IsClassDecl() { return false; }
    virtual bool IsInterfaceDecl() { return false; }
    virtual bool IsFnDecl() { return false; }
    virtual bool IsMethodDecl() { return false; }
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/// VarDecl
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class VarDecl : public Decl
{
  protected:
    Type *type;
    //bool type_declared;

  public:
    VarDecl(Identifier *name, Type *type);

    bool AreEquiv(Decl *other);

    Type* TypeFinder() { return type; }
    void Check();
    //bool is_declared_type() { return type_declared; }


  private:
    void FindType();
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/// ClassDecl
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class ClassDecl : public Decl
{
  protected:
    List<Decl*> *members;
    NamedType *extends;
    List<NamedType*> *implements;

  public:
    ClassDecl(Identifier *name, NamedType *extends,
              List<NamedType*> *implements, List<Decl*> *members);
    void ScopeMake(Scope *parent);
    void Check();
    NamedType* TypeFinder() { return new NamedType(id); }
    NamedType* GetExtends() { return extends; }
    List<NamedType*>* GetImplements() { return implements; }

  private:
    void ExtendsFinder();
    void ImplementsFinder();
    void ExtendedMembersFinder(NamedType *extType);
    void ImplementedMembersFinder(NamedType *impType);
    void AgScopeFinder(Scope *other);
    void ImplementsFinderInterfaces();
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/// InterfaceDecl
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class InterfaceDecl : public Decl
{
  protected:
    List<Decl*> *members;

  public:
    InterfaceDecl(Identifier *name, List<Decl*> *members);
    void ScopeMake(Scope *parent);
    void Check();
    Type* TypeFinder() { return new NamedType(id); }
    List<Decl*>* GetMembers() { return members; }
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/// FnDecl
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class FnDecl : public Decl
{
  protected:
    List<VarDecl*> *formals;
    Type *returnType;
    Stmt *body;

  public:
    FnDecl(Identifier *name, Type *returnType, List<VarDecl*> *formals);
    void SetFunctionBody(Stmt *b);
    bool AreEquiv(Decl *other);
    Type* GetReturnType() { return returnType; }
    List<VarDecl*>* GetFormals() { return formals; }
    void ScopeMake(Scope *parent);
    void Check();
};

#endif
