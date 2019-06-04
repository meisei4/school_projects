/* Semantics.c
   Support and semantic action routines.

*/


#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "YSemantics.h"


struct SymTab * IdentifierTable;
struct SymTab * StrLitTable;
enum AttrKinds { VOID_KIND = -1, INT_KIND, STRING_KIND, STRUCT_KIND };

char * BaseTypeNames[2] = { "int", "chr"};
char * TypeNames[2] = { "", "func"};

// corresponds to enum Operators
char * Ops[] = { "add", "sub", "mul", "div"};
//char * BoolOps[] = { "and", "or", "not" };

// corresponds to negation of enum Comparisons
// enum Comparisons { LtCmp, LteCmp, GtCmp, GteCmp, EqCmp, NeqCmp };
char * Branches[] = { "bge", "bgt", "ble", "blt", "bne", "beq"};
// comparison set instructions, in order with enum Comparisons
//char * CmpSetReg[] = { "slt", "sle", "sgt", "sge", "seq", "sne" };

// Supporting Routines
void
PostMessageAndExit(int col, char * message) {
    PostMessage(col,1,message);
    CloseSource();
    exit(0);
}

void
InitSemantics() {
    IdentifierTable = CreateSymTab(100,"global",NULL);
    StrLitTable = CreateSymTab(100,"global", NULL);
}

char *
StringForType(struct TypeDesc * desc) {
    switch (desc->declType) {
        case PrimType: {
                           return strdup(BaseTypeNames[desc->primDesc]);
                       } break;
        case FuncType: {
                           return strdup(BaseTypeNames[desc->funcDesc->returnType]);
                       } break;
    }
}

struct TypeDesc *
MakePrimDesc(enum BaseTypes type) {
    struct TypeDesc * desc = malloc(sizeof(struct TypeDesc));
    desc->declType = PrimType;
    desc->primDesc = type;
    return desc;
}

struct TypeDesc *
MakeFuncDesc(enum BaseTypes returnType) {
    struct TypeDesc * desc = malloc(sizeof(struct TypeDesc));
    desc->declType = FuncType;
    desc->funcDesc = malloc(sizeof(struct FuncDesc));
    desc->funcDesc->returnType = returnType;
    desc->funcDesc->funcCode = NULL;
    return desc;
}

void displayEntry(struct SymEntry * entry, int cnt, void * ignore) {
    char * scope = GetScopePath(GetTable(entry));
    printf("%3d %-20s %-15s",cnt,scope,GetName(entry));
    free(scope);
    int attrKind = GetAttrKind(entry);

    switch (attrKind) {
        case VOID_KIND: {
                        
                        } break;
        case INT_KIND: {
                       } break;
        case STRING_KIND: { 
                              struct StrLitAttr * attr = GetAttr(entry);
                              if (attr) {
                                  printf("%-10s", attr->label);
                                  printf("     ");
                              }
                          } break;
        case STRUCT_KIND: {
                              struct Attr * attr = GetAttr(entry);
                              if (attr) {
                                  printf("%-10s",attr->reference);
                                  printf("     ");
                                  struct TypeDesc * desc = attr->typeDesc;
                                  char * typeStr = StringForType(desc);
                                  printf("%-10s ",typeStr);
                                  free(typeStr);
                              }
                              else {
                                  printf("empty");
                              }
                          } break;
    }
    printf("\n");
}

void
ListIdentifierTable() {
    printf("\nIdentifier Symbol Table\n");
    printf("Num Scope                Name           Ref       Kind Type\n");
    InvokeOnEntries(IdentifierTable,true,displayEntry,0,NULL);
}

void 
ListStringTable(){
    printf("\nString Lit Symbol Table\n");
    printf("Num Scope                Name           Lbl\n");
    InvokeOnEntries(StrLitTable,true,displayEntry,0,NULL);
}


void
FreeTypeDesc(struct TypeDesc * desc) {
    switch (desc->declType) {
        case PrimType: {
                       } break;
        case FuncType: {
                           if (desc->funcDesc->funcCode) FreeSeq(desc->funcDesc->funcCode);
                           free(desc->funcDesc);
                       } break;
    }
    free(desc);
}

// provided to the SymTab for destroying table contents
void
FreeEntryAttr(struct SymEntry * entry) {
    int attrKind = GetAttrKind(entry);
    struct Attr * attr = GetAttr(entry);
    switch (attrKind) {
        case VOID_KIND: {
                        } break;
        case INT_KIND: {
                       } break;
        case STRING_KIND: {
                          } break;
        case STRUCT_KIND: {
                              if (attr) {
                                  if (attr->typeDesc)  FreeTypeDesc(attr->typeDesc);
                                  if (attr->reference) free(attr->reference);
                                  free(attr);
                              }
                          } break;
    }
}

void processString(struct SymEntry * entry, int cnt, void * dataCode) {
    struct StrLitAttr * attr = GetAttr(entry);
    dataCode = AppendSeq(dataCode, GenInstr(attr->label, ".asciiz",  (char *)GetName(entry), NULL, NULL)); 
} 

void processGlobalIdentifier(struct SymEntry * entry, int cnt, void * dataCode) {
    struct Attr * attr = GetAttr(entry);
    switch (attr->typeDesc->declType) {
        case PrimType: {
                        dataCode = AppendSeq(dataCode, GenInstr(attr->reference, ".word", "0", NULL, NULL));
                       } break;
        case FuncType: {
                        // nothing to do here
                       } break;
    }
}

void processFunctions(struct SymEntry * entry, int cnt, void * textCode) {
    struct Attr * attr = GetAttr(entry);
    switch (attr->typeDesc->declType) {
        case PrimType: {
                        // nothing to do here
                       } break;
        case FuncType: {
                            if (!attr->typeDesc->funcDesc->funcCode) {
                                PostMessageAndExit(tokStrt,"function never implemented");
                            } else {
                                textCode = AppendSeq(textCode, attr->typeDesc->funcDesc->funcCode);
                            }
                        } break;
    }
}

// Semantics Actions

void
Finish() {
    printf("Finish\n");
    ListIdentifierTable();
    ListStringTable();
    struct InstrSeq * textCode = GenInstr(NULL,".text",NULL,NULL,NULL);
    struct InstrSeq * dataCode = GenInstr(NULL,".data",NULL,NULL,NULL);

    // form module preamble
    struct SymEntry * mainEntry = LookupName(IdentifierTable,"main");
    if (!mainEntry) {
        PostMessageAndExit(tokStrt,"no main function for module");
    }
    // should make sure main takes no arguments
    struct Attr * mainAttr = GetAttr(mainEntry);
    // need to keep spim happy
    AppendSeq(textCode,GenInstr(NULL,".globl","__start",NULL,NULL));
    AppendSeq(textCode,GenInstr("__start",NULL,NULL,NULL,NULL));
    AppendSeq(textCode,GenInstr(NULL,"jal",mainAttr->reference,NULL,NULL));
    AppendSeq(textCode,GenInstr(NULL,"li","$v0","10",NULL));
    AppendSeq(textCode,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    // run SymTab with InvokeOnEntries putting globals in data seg
    InvokeOnEntries(IdentifierTable, true, processGlobalIdentifier, 0, dataCode);
    InvokeOnEntries(StrLitTable, true, processString, 0, dataCode);
    // run SymTab with InvokeOnEntries putting functions in code seq
    InvokeOnEntries(IdentifierTable, true, processFunctions, 0, textCode);
 
    // combine and write
    struct InstrSeq * moduleCode = AppendSeq(textCode,dataCode);
    WriteSeq(moduleCode);

    // free code
    FreeSeq(moduleCode);
    CloseCodeGen();
    fprintf(stderr,"Finish \n");
}

//////////////////////////////////////////////////////////SEMANTICS 2下///////////////////////////////////////////////////
struct InstrSeq * 
Crement(char * id, char * opcode){
    int reg = AvailTmpReg();
    struct SymEntry * entry = LookupName(IdentifierTable, id);
    struct Attr * attr = GetAttr(entry);
    char * ref = attr->reference;
    struct InstrSeq * instr = GenInstr(NULL, "lw", TmpRegName(reg), ref , NULL);
    instr = AppendSeq(instr, GenInstr(NULL, opcode, TmpRegName(reg), TmpRegName(reg), "1"));
    instr = AppendSeq(instr, GenInstr(NULL, "sw", TmpRegName(reg), ref, NULL));
    return instr;
}

struct InstrSeq *
For(struct InstrSeq * assign, struct CondResult * cond, struct InstrSeq * crement, struct InstrSeq * forbody){
    char * label = GenLabel();
    struct InstrSeq * instrs = AppendSeq(assign, GenInstr(label, NULL, NULL, NULL, NULL));
    AppendSeq(instrs, cond->code);
    AppendSeq(instrs, forbody);
    AppendSeq(instrs, crement);
    AppendSeq(instrs, GenInstr(NULL, "j", label, NULL, NULL));
    AppendSeq(instrs, GenInstr(cond->label, NULL, NULL, NULL, NULL));
    return instrs;
}

//////////////////////SEMANTICS 2 上////////////////////////////////////SEMANTICS 1下/////////////////////////////////////
struct CondResult * 
Compare(struct ExprResult * oprd1, struct ExprResult * oprd2, char * branch){
    struct CondResult * condRes = malloc(sizeof(struct CondResult));
    char * label = GenLabel();
    struct InstrSeq * instr = AppendSeq(oprd1->code, oprd2->code);
    instr = AppendSeq(instr, GenInstr(NULL, branch, TmpRegName(oprd1->reg), TmpRegName(oprd2->reg), label));
    condRes->code = instr;
    condRes->label = label; //Label to jump to when the comparison is true thus use the opposite branch type to jump to an else
    return condRes;
}

//make an IF ELSE INSTRUCTION SEQUENCE
struct InstrSeq * 
IfBranch(struct CondResult * cond, struct InstrSeq * ifBody, struct InstrSeq * elseBody){
    char * label;
    struct InstrSeq * instr;
    if(!cond || !ifBody){
        return NULL;
    } else if(!elseBody){ //IF NO ELSEBODY branch on failure to ENDIF LABEL
        instr = AppendSeq(cond->code, ifBody);
        instr = AppendSeq(instr, GenInstr(cond->label, NULL, NULL, NULL, NULL)); //LABEL BRANCHED TO IF COND IS FALSE
        return instr;
    }
    instr = AppendSeq(cond->code, ifBody);
    label = GenLabel(); //LABEL TO SKIP OVER THE ELSEBODY
    instr = AppendSeq(instr, GenInstr(NULL, "j", label, NULL, NULL)); //IF REACHED HERE JUMP OVER ELSE BODY TO ENDIF LABEL
    instr = AppendSeq(instr, GenInstr(cond->label, NULL, NULL, NULL, NULL)); //APPEND THE ELSE LABEL
    instr = AppendSeq(instr, elseBody); //APPEND THE ELSE BODY
    instr = AppendSeq(instr, GenInstr(label, NULL, NULL, NULL, NULL)); //APPEND THE LABEL CREATED IN ORDER TO SKIP ELSE BODY
    return instr;
}

//MAKE WHILE INSTRUCTION SEQ
struct InstrSeq * 
While(struct CondResult * cond, struct InstrSeq * whilebody){
    if(!cond || !whilebody){
        return NULL;
    }
    char * label = GenLabel();
    struct InstrSeq * instr = GenInstr(label, NULL, NULL, NULL, NULL); //ADD LOOP TO LABEL
    instr = AppendSeq(instr, cond->code); //ADD BRANCH CONDITION
    instr = AppendSeq(instr, whilebody); //ADD WHILE BODY
    instr = AppendSeq(instr, GenInstr(NULL, "j", label, NULL, NULL)); //ADD JUMP BACK TO LOOP LABEL
    instr = AppendSeq(instr, GenInstr(cond->label, NULL, NULL, NULL, NULL)); //ADD EXIT LABEL
    return instr;
}

char *
GetAttrRefId(char * token){
    struct SymEntry * id = LookupName(IdentifierTable, token);
    struct Attr * attr = GetAttr(id);
    return attr->reference;
}

char * 
GetAttrLabel(char * token){
    struct SymEntry * str = LookupName(StrLitTable, token);
    struct StrLitAttr * attr = GetAttr(str);
    return attr->label;
}

enum BaseTypes 
GetBaseType(char * token){
    struct SymEntry * id = LookupName(IdentifierTable, token);
    struct Attr * attr = GetAttr(id);
    return attr->typeDesc->primDesc;
}

//GET FUNCTIONALITY
struct ExprResult *
Get(){
    struct InstrSeq * instr = GenInstr(NULL, "li", "$v0", "5", NULL);
    struct ExprResult * exp = malloc(sizeof(struct ExprResult));
    exp->type = IntBaseType;
    exp->code = AppendSeq(instr, GenInstr(NULL, "syscall", NULL, NULL, NULL));
    int reg = AvailTmpReg();
    char * regname = TmpRegName(reg);
    exp->code = AppendSeq(exp->code, GenInstr(NULL, "move", regname, "$v0", NULL));
    exp->reg = reg;
    return exp;
}

//to check if token is a number
int 
isnumber(char * token){
    for(int i = 0; i < strlen(token); i++){
        if(!isdigit(token[i])){
            return 0;
        }
    }
    return 1;
}

struct InstrSeq * 
Put(struct ExprResult * exp){
    char * syscallCode;
    struct InstrSeq * instr = exp->code;
    switch(exp->type){
        case IntBaseType : syscallCode = "1";
                           break;
        case ChrBaseType : syscallCode = "11";
                           break;
        case StrBaseType : syscallCode = "4";
                           break;
    }
    instr = AppendSeq(instr, GenInstr(NULL, "li", "$v0", syscallCode, NULL));
    instr = AppendSeq(instr, GenInstr(NULL, "move", "$a0", TmpRegName(exp->reg), NULL));
    ReleaseTmpReg(exp->reg);
    instr = AppendSeq(instr, GenInstr(NULL, "syscall", NULL, NULL, NULL));
    return instr; 
}

struct ExprResult * 
Negate(struct ExprResult * exp){
    struct InstrSeq * instr = GenInstr(NULL, "mul", TmpRegName(exp->reg), TmpRegName(exp->reg), "-1");
    exp->code = AppendSeq(exp->code, instr);
    return exp;
}

struct ExprResult * 
Load(char * token, enum BaseTypes type){
    char * opcode;
    char * oprnd;
    if(isnumber(token) || !(token[0] - '\'')) {
        opcode = "li";
        oprnd = token;
    } else {
        opcode = "lw";
        oprnd = GetAttrRefId(token);
    }
    int reg = AvailTmpReg();
    char * regname = TmpRegName(reg);
    struct InstrSeq * instr = GenInstr(NULL, opcode, regname, oprnd, NULL);
    struct ExprResult * exprRes = malloc(sizeof(struct ExprResult));
    exprRes->type = type;
    exprRes->code = instr;
    exprRes->reg = reg;
    return exprRes;
}

struct InstrSeq * 
Store(char * token, struct ExprResult * exprRes){
    char * ref = GetAttrRefId(token);
    struct InstrSeq * instr = GenInstr(NULL, "sw", TmpRegName(exprRes->reg), ref, NULL);
    //RELEASE
    ReleaseTmpReg(exprRes->reg);
    exprRes->code = AppendSeq(exprRes->code, instr);
    return exprRes->code;
}

struct ExprResult * 
Arithmetic(struct ExprResult * opr1, struct ExprResult * opr2, enum Op op, enum BaseTypes type){
    int reg = AvailTmpReg();
    char * regname = TmpRegName(reg);
    char * opcode;
    switch(op){
        case Add: opcode = "add"; 
            break;
        case Sub: opcode = "sub";
            break;
        case Mult: opcode = "mul";
            break;
        case Div: opcode = "div";
            break;
    }
    struct InstrSeq * instr = GenInstr(NULL, opcode, regname, TmpRegName(opr1->reg), TmpRegName(opr2->reg));
    struct ExprResult * exprRes = malloc(sizeof(struct ExprResult));
    exprRes->code = AppendSeq(opr1->code, opr2->code);
    //RELEASE 
    ReleaseTmpReg(opr1->reg);
    ReleaseTmpReg(opr2->reg);
    exprRes->type = type;
    exprRes->code = AppendSeq(exprRes->code, instr);
    exprRes->reg = reg;
    return exprRes;
}

void
ProcDecls(struct IdList * idList, enum BaseTypes baseType) {
    while(idList){
        const char * name = GetName(idList->entry);
        struct Attr * attr = GetAttr(idList->entry);
        enum DeclTypes dt = attr->typeDesc->declType;
        switch(dt){
            case PrimType : {     
                                struct TypeDesc * td = MakePrimDesc(baseType);
                                attr->typeDesc = td;
                            } break;
            case FuncType : {
                                struct TypeDesc * td = MakeFuncDesc(baseType);
                                attr->typeDesc = td;
                            } break;
            default : printf("ERROR"); 
                      exit(-1);
        }
        char * ref = malloc(sizeof(name) + sizeof(char));
        sprintf(ref, "_%s", name);
        attr->reference = ref;
        idList = idList->next;
    }
    // walk IdList items
    // switch for prim or func type
    // names on IdList are only specified as PrimType or FuncType
    // set type desc
    // for Sem1 everthing is in global scope, otherwise check scope
    // attr reference string is id name with prepended "_"
}

struct IdList *
AppendIdList(struct IdList * item, struct IdList * list) {
    item->next = list;
    return item;
}

struct ExprResult *
ProcString(char * str){
    struct ExprResult * exp = malloc(sizeof(struct ExprResult));
    int reg = AvailTmpReg();
    if(LookupName(StrLitTable, str)){
        exp->code = GenInstr(NULL, "la", TmpRegName(reg), GetAttrLabel(str), NULL);
        exp->reg = reg;
        exp->type = StrBaseType;
        return exp;
    }
    char * label = GenLabel();
    struct SymEntry * entry = EnterName(StrLitTable, str);
    struct StrLitAttr * attr = malloc(sizeof(struct StrLitAttr));
    attr->label = label;
    SetAttr(entry, STRING_KIND, attr);
    exp->code = GenInstr(NULL, "la", TmpRegName(reg), label, NULL);
    exp->type = StrBaseType;
    exp->reg = reg;
    return exp;
}

struct IdList * 
ProcName(char * id, enum DeclTypes type) {
    if(LookupName(IdentifierTable, id)){
        fprintf(stderr, "ERROR at PROCNAME\n");
        exit(-1);
    }
    struct SymEntry * entry = EnterName(IdentifierTable, id);
    struct IdList * node = malloc(sizeof(struct IdList));
    struct Attr * attr = malloc(sizeof(struct Attr));
    struct TypeDesc * td = malloc(sizeof(struct TypeDesc));
    td->declType = type;
    attr->typeDesc = td;
    SetAttr(entry, STRUCT_KIND, attr);
    node->entry = entry;
    return node;
    // get entry for id, error if it exists
    // enter id in symtab
    // create IdList node for entry
    // create and partially init type descriptor, completed in ProcDecls
    // create, init and set attr struct
}

void
ProcFunc(char * id, struct InstrSeq * instrs) {
    struct SymEntry * entry = LookupName(IdentifierTable, id);
    if(!entry){
        fprintf(stderr, "ERROR at PROCFUNC\n");
        exit(-1);
        return;
    }
    struct Attr * attr = GetAttr(entry);
    instrs = AppendSeq(GenInstr(attr->reference, NULL, NULL, NULL, NULL), instrs);
    instrs = AppendSeq(instrs, GenInstr(NULL, "jr", "$ra", NULL, NULL));
    attr->typeDesc->funcDesc->funcCode = instrs;
    // lookup name
    // get attr
    // gen instr for function entry
    // include function body code
    // function exit code, i.e. jump return
}
