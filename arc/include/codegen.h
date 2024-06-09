#ifndef CODEGEN_H
#define CODEGEN_H

#include "asa.h"
#include "ram_os.h"

extern FILE * exefile;




void codegen(asa *p);

void codeID(asa* p); //OKAY
void codeNB(asa * p); //OKAY
void codeOP(asa * p); //OKAy
void codeDecla(asa *p); //OKAY
void codeTQ(asa* p); //OKAY
void codeAffect(asa* p); // OKAY  
void codeEcrire(asa* p);
void codeLire(asa* p);
void codePour(asa* p);
void codeSi(asa* p);
void codeListInstruction(asa* p); 
void codeMain(asa* p); 
void codeListDecla(asa* p);
void codeRenvoyer(asa* p);
void codeAlgo(asa* p);



#endif
