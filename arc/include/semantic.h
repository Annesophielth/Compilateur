#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ts.h"
#include "asa.h"
#include "codegen.h"

#define CTXGLB "GLOBAL"

extern char CTXT[32];  // Déclaration de la variable CTXT
extern int pile_CTXT_main;

void semantic(asa *p);
void semantic_ID(asa* p); //OKAY
void semantic_NB(asa * p); //OKAY
void semantic_OP(asa * p); //OKAy
void semantic_Decla_var(asa *p); //OKAY
void semantic_Algo(asa* p);
void semantic_Si(asa* p);
void semantic_Ecrire(asa* p);
void semantic_Lire(asa* p);
void semantic_Main(asa* p); //OKAY
void semantic_ListDecla(asa* p); //OKAY
void semantic_Pour(asa* p);
void semantic_TQ(asa* p); //CHECK
void semantic_Affect(asa* p); //OKAY
void semantic_Renvoyer(asa* p);
void semantic_ListInstruction(asa* p); //OKAY
extern table_symb TABLE_SYMBOLES;

#endif

