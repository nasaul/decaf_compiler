/* File: scope.h
 * -------------
 * The Scope class will be used to manage scopes, sort of
 * table used to map identifier names to Declaration objects.
 */

#ifndef _H_scope
#define _H_scope

#include "hashtable.h"

class Decl;
class Identifier;
class ClassDecl;
class LoopStmt;
class SwitchStmt;
class FnDecl;


class Scope
{
  private:
    Scope *parent;

  public:
    Hashtable<Decl*> *table;
    ClassDecl *classDecl;
    LoopStmt *loopStmt;
    SwitchStmt *switchStmt;
    FnDecl *fnDecl;

  public:
    Scope() : table(new Hashtable<Decl*>), classDecl(NULL), loopStmt(NULL),
              fnDecl(NULL) {}

    void SetParent(Scope *p) { parent = p; }
    Scope* GetParent() { return parent; }

    void SetClassDecl(ClassDecl *d) { classDecl = d; }
    ClassDecl* Get_Class_Declaration() { return classDecl; }

    void SetLoopStmt(LoopStmt *s) { loopStmt = s; }
    LoopStmt* GetLoopStmt() { return loopStmt; }

    void SetSwitchStmt(SwitchStmt *s) { switchStmt = s; }
    SwitchStmt* GetSwitchStmt() { return switchStmt; }

    void SetFnDecl(FnDecl *d) { fnDecl = d; }
    FnDecl* GetFnDecl() { return fnDecl; }

    int Add_Declaration(Decl *decl);
    friend std::ostream& operator<<(std::ostream& out, Scope *s);
    bool Declare(Decl *dec);
    void CopyFromScope(Scope *other, ClassDecl *cd);
    Decl *Lookup(Identifier *id);
};

#endif
