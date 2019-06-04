/* Semantics.h
   The action and supporting routines for performing semantics processing.
   */

#include "SymTab.h"
#include "IOMngr.h"
#include "Scanner.h"
#include "YCodeGen.h"

/*トラクト */

enum Op {Add, Div, Sub, Mult };

enum BaseTypes { IntBaseType, ChrBaseType, StrBaseType };

struct ExprResult {
    struct InstrSeq * code;
    enum BaseTypes type;
    int reg;
};

struct CondResult {
    struct InstrSeq * code;
    char * label;
};

/* Declaration of semantic record data types used in grammar.y %union */
struct IdList {
    struct SymEntry * entry;
    struct IdList * next;
};

struct FuncDesc {
    enum BaseTypes returnType;
    struct InstrSeq * funcCode;
};

enum DeclTypes { PrimType, FuncType };
struct TypeDesc {
    enum DeclTypes declType;
    union {
        enum BaseTypes primDesc;
        struct FuncDesc * funcDesc;
    };
};

// Symbol Table Structures
struct Attr {
    struct TypeDesc * typeDesc;
    char * reference;
};

struct StrLitAttr {
    char * label;
};

// Supporting Routines

void InitSemantics();
void ListIdentifierTable();

struct TypeDesc * MakePrimDesc(enum BaseTypes type);
struct TypeDesc * MakeFuncDesc(enum BaseTypes returnType);

// Semantics Actions
void                    Finish();
struct ExprResult *     ProcString(char * str);
void                    ProcDecls(struct IdList * idList, enum BaseTypes baseType);
struct IdList *         AppendIdList(struct IdList * item, struct IdList * list);
struct IdList *         ProcName(char * id, enum DeclTypes type);
void                    ProcFunc(char * id, struct InstrSeq * instrs);
//SEM 1
struct ExprResult *     Arithmetic(struct ExprResult * opr1, struct ExprResult * opr2, enum Op op, enum BaseTypes type);
struct InstrSeq *       Store(char * token, struct ExprResult * exprRes);
struct ExprResult *     Load(char * token, enum BaseTypes type);
struct ExprResult *     Get();
struct ExprResult *     Negate(struct ExprResult * exp);
struct InstrSeq *       Put(struct ExprResult * exp);
struct CondResult *     Compare(struct ExprResult * oprd1, struct ExprResult * oprd2, char * branch);
struct InstrSeq *       IfBranch(struct CondResult * cond, struct InstrSeq * ifBody, struct InstrSeq * elseBody);
struct InstrSeq *       While(struct CondResult * cond, struct InstrSeq * whileBody); 
//SEM 2
struct InstrSeq *       Crement(char * id, char * updown);
struct InstrSeq *       For(struct InstrSeq * assign, struct CondResult * cond, struct InstrSeq * crement, struct InstrSeq * forbody);
