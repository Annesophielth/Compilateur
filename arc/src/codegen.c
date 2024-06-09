#include "codegen.h"
#include "ts.h"
#include "semantic.h"



int codelen = 0;
int nb_ligne = 0;


//Fonction pour connaitre la taille du code
int parcours_asa(asa* p)
{
  switch (p->type)
  {
  case typeID:
    p -> codelen = 1;
    break;

  case typeOP:
    p -> codelen = 7 + parcours_asa(p -> op.noeud[0]) + parcours_asa(p -> op.noeud[1]);
    break;

  case typeLire:
    p -> codelen = 2 + parcours_asa(p -> lire.ele);
    break;

  case typeEcrire:
    p -> codelen = 2 + parcours_asa(p -> ecrire.ele);
    break;

  case typeNB:
    p -> codelen = 1;
    break;

  case typeDecla:
    p -> codelen = 2 + parcours_asa(p -> decla.declaration) + parcours_asa(p -> decla.decla_suivant);
    break;

  case typeAffect:
    p -> codelen = 2 + parcours_asa(p -> affect.gauche) + parcours_asa(p -> affect.droite);
    break;

  case typeMain:
    p -> codelen = parcours_asa(p -> main.decla) + parcours_asa(p -> main.instruct) + parcours_asa(p -> main.re);
    break;

  case typeRenvoyer:
    p -> codelen = 2 + parcours_asa(p -> renvoyer.ele);
    break;


  case typeListInstruction:
    if (p -> listinstruction.suivant != NULL)
      p -> codelen = parcours_asa(p -> listinstruction.instruct) + parcours_asa(p -> listinstruction.suivant);
    else
      p -> codelen = parcours_asa(p -> listinstruction.instruct);
    break;

  case typeListDecla:
    if (p -> listdecla.suivant != NULL)
      p -> codelen = parcours_asa(p -> listdecla.decla) + parcours_asa(p -> listdecla.suivant);
    else
      p -> codelen = parcours_asa(p -> listdecla.decla);
    break;

  case typeAlgo:
    p -> codelen = 4 + parcours_asa(p -> algo.decla) + parcours_asa(p -> algo.main);
    break;

  case typeSi:
    p -> codelen = 1 + parcours_asa(p -> si.exp) + parcours_asa(p -> si.si_instruct);    
        break;

  case typePour:
    p -> codelen =  2 + parcours_asa(p -> pour.exp) + parcours_asa(p -> pour.instruct);
    break;

  case typeTQ:
    p -> codelen = 2 + parcours_asa(p -> tq.exp) + parcours_asa(p -> tq.instruct);  
    break;

  default:
    break;
  }
  return p -> codelen;
}





void codegen(asa *p)
{
  if(!p) 
    return;

  switch (p->type)
  {
    case typeNB:
      printf("CODE NB\n");
      codeNB(p);
      break;
    
    case typeOP:
      printf("CODE OP\n");
      codeOP(p);
      break;

    case typeID:
      printf("CODE ID\n");
      codeID(p);
      break;

    case typeDecla:
      printf("CODE DECLA\n");
      codeDecla(p);
      break;

    case typeAffect:
      printf("CODE AFFECT\n");
      codeAffect(p);
      break;

    case typeMain:
      codeMain(p);
      break;

    case typeListInstruction:
      codeListInstruction(p);
      break;

    case typeRenvoyer:
      printf("CODE RENVOYER\n");
      codeRenvoyer(p);
      break;


    case typeListDecla:
      printf("CODE LISTE DECLA\n");
      codeListDecla(p);
      break;


    case typeAlgo:
      printf("\n\t####  PARTIE CODEGEN ####\n");
      printf("CODE ALGO\n");
      codeAlgo(p);
      break;

    case typeSi:
      printf("CODE SI\n");
      codeSi(p);
      break;

    case typeEcrire:
      printf("CODE ECRIRE\n");
      codeEcrire(p);
      break;

    case typeLire:
      printf("CODE LIRE\n");
      codeLire(p);
      break;

    case typeTQ:
      printf("CODE TQ\n");
      codeTQ(p);
      break;

    case typePour:
      printf("CODE POUR\n");
      codePour(p);
      break;

    default:
      break;
  }
}


void codeEcrire(asa* p)
{
  symbole* ele = ts_rechercher_identificateur(TABLE_SYMBOLES, p -> ecrire.ele -> id.nom, CTXT);
  fprintf(exefile, "LOAD %d;\n", ele -> adr);
  fprintf(exefile, "WRITE; CODE ECRIRE\n\n");
  nb_ligne += 2;
}

void codeLire(asa* p)
{
  symbole* ele = ts_rechercher_identificateur(TABLE_SYMBOLES, p -> ecrire.ele -> id.nom, CTXT);
  fprintf(exefile, "READ;\n");
  fprintf(exefile, "STORE %d; CODE LIRE\n\n", ele -> adr);  
  nb_ligne += 2;
}


void codeAlgo(asa* p)
{
  //ADRESSE POUR LES CALCULS
  fprintf(exefile, "LOAD #32;\n");
  fprintf(exefile, "STORE 2;\n");
  fprintf(exefile, "LOAD #0;\n");
  fprintf(exefile, "STORE 32; CODE ALGO\n\n");
  nb_ligne += 4;


  if (p -> algo.decla != NULL)
    codegen(p -> algo.decla);
  codegen(p -> algo.main);
}

void codeListDecla(asa* p)
{
  codegen(p -> listdecla.decla);

  if (p -> listdecla.suivant != NULL)
    codegen(p -> listdecla.suivant);
}


//IL Y A UN SOUCIS !!!!
void codeRenvoyer(asa* p)
{
  symbole* ele = ts_rechercher_identificateur(TABLE_SYMBOLES, p -> renvoyer.ele -> id.nom, CTXT);
  int adr = ele -> adr;

  fprintf(exefile, "LOAD %d;\n", adr); //Copie le regestre %d
  fprintf(exefile, "STORE %d;CODE RENVOYER\n\n", adr); //
  nb_ligne +=2;
}


void codeListInstruction(asa* p)
{
  codegen(p -> listinstruction.instruct);

  if (p -> listinstruction.suivant != NULL)
    codegen(p -> listinstruction.suivant);
}


void codeMain(asa* p)
{
  strcpy(CTXT, "MAIN");

  if (p -> main.decla != NULL)
    codegen(p -> main.decla);

  codegen(p -> main.instruct);

  strcpy(CTXT, "GLOBAL");
}


void codeNB(asa * p)
{
  //on stocke la valeur de l'entier dans l'ACC
  fprintf(exefile,"LOAD #%d; CODE NB \n\n", p -> nb.val);
  nb_ligne +=1;
}


void codeOP(asa * p)
{
  /*
   * On commence par générer le code des noeuds dans l'ordre de l'associativité
   */
  switch (p->op.ope)
  {
    /*
     * Operateurs associatifs à gauche, on génère d'abord la partie droite
     * on l'empile et on génère la partie gauche
      ON LE FAIT POUR TOUS LES OPÉRATEURS MAIS PAS BESOIN DE SPÉCIFIER CAR ON FAIT 
      JUSTE LA PARTIE DE DROITE
    */
  case '+': 
    codegen(p->op.noeud[1]); 
    
    fprintf(exefile,"INC %d;\n", RAM_OS_STK_REG); //RAM_OS_STK_REG incrémentation de la valeur de la pile à 1
    fprintf(exefile, "STORE @%d ; CODE OP\n\n", RAM_OS_STK_REG);

    codegen(p->op.noeud[0]);

    fprintf(exefile, "ADD @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "STORE 1;\n");
    fprintf(exefile, "LOAD #0;\n");
    fprintf(exefile, "STORE @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "LOAD 1; CALCUL ADDITION\n\n");
    nb_ligne +=7;
    break;
    
  case '-':
    codegen(p->op.noeud[1]); 
    
    fprintf(exefile,"INC %d;\n", RAM_OS_STK_REG); //RAM_OS_STK_REG incrémentation de la valeur de la pile à 1
    fprintf(exefile, "STORE @%d ;CODE OP\n\n", RAM_OS_STK_REG);

    codegen(p->op.noeud[0]);

    fprintf(exefile, "SUB @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "STORE 1;\n");
    fprintf(exefile, "LOAD #0;\n");
    fprintf(exefile, "STORE @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "LOAD 1; CALCUL SOUSTRACTION\n\n");
    nb_ligne +=7;
    break;

  case '*':
    codegen(p->op.noeud[1]); 
    
    fprintf(exefile,"INC %d;\n", RAM_OS_STK_REG); //RAM_OS_STK_REG incrémentation de la valeur de la pile à 1
    fprintf(exefile, "STORE @%d ; CODE OP\n\n", RAM_OS_STK_REG);

    codegen(p->op.noeud[0]);

    fprintf(exefile, "MUL @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "STORE 1;\n");
    fprintf(exefile, "LOAD #0;\n");
    fprintf(exefile, "STORE @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "LOAD 1; CALCUL MULTIPLICATION\n\n");
    nb_ligne +=7;
    break;

  case '/':
    codegen(p->op.noeud[1]); 
    
    fprintf(exefile,"INC %d;\n", RAM_OS_STK_REG); //RAM_OS_STK_REG incrémentation de la valeur de la pile à 1
    fprintf(exefile, "STORE @%d ;CODE OP\n\n", RAM_OS_STK_REG);

    codegen(p->op.noeud[0]);

    fprintf(exefile, "DIV @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "STORE 1;\n");
    fprintf(exefile, "LOAD #0;\n");
    fprintf(exefile, "STORE @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "LOAD 1; CALCUL DE DIVISION\n\n");
    nb_ligne +=7;
    break;

  case '%':
    codegen(p->op.noeud[1]); 
    
    fprintf(exefile,"INC %d;\n", RAM_OS_STK_REG); //RAM_OS_STK_REG incrémentation de la valeur de la pile à 1
    fprintf(exefile, "STORE @%d ;CODE OP\n\n", RAM_OS_STK_REG);

    codegen(p->op.noeud[0]);

    fprintf(exefile, "MOD @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "STORE 1;\n");
    fprintf(exefile, "LOAD #0;\n");
    fprintf(exefile, "STORE @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "LOAD 1; CALCUL DU MODULO\n\n");   
    nb_ligne +=7;
    break; 

  case '<': //Inférieur
    codegen(p->op.noeud[1]); 
    
    fprintf(exefile,"INC %d;\n", RAM_OS_STK_REG); //RAM_OS_STK_REG incrémentation de la valeur de la pile à 1
    fprintf(exefile, "STORE @%d ;CODE OP\n\n", RAM_OS_STK_REG);

    codegen(p->op.noeud[0]);

    //Je soustrait
    fprintf(exefile, "SUB @%d; CODE DE =\n", RAM_OS_STK_REG);
    fprintf(exefile, "STORE @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "INC %d; SOUSTRACTION EFFECTUÉ noeud[0] - noeud[1] < 0 ?\n\n", RAM_OS_STK_REG);

    nb_ligne += 5;
    fprintf(exefile, "JUML %d;\n", nb_ligne + 3); //Il faut aller à l'instruction dans le TQ
    fprintf(exefile, "LOAD #0;\n");
    fprintf(exefile, "JUMP %d;\n", nb_ligne + 4);
    fprintf(exefile, "LOAD #1;\n");
    nb_ligne +=4;
    break;

  case '>': //Suppèrieur
    codegen(p->op.noeud[1]); 
    
    fprintf(exefile,"INC %d;\n", RAM_OS_STK_REG); //RAM_OS_STK_REG incrémentation de la valeur de la pile à 1
    fprintf(exefile, "STORE @%d ;CODE OP\n\n", RAM_OS_STK_REG);

    codegen(p->op.noeud[0]);

    //Je commance par soustraire
    fprintf(exefile, "SUB @%d; CODE DE =\n", RAM_OS_STK_REG);
    fprintf(exefile, "STORE @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "INC %d; SOUSTRACTION EFFECTUÉ noeud[0] - noeud[1] > 0 ?\n\n", RAM_OS_STK_REG);

    nb_ligne += 5;
    fprintf(exefile, "JUMG %d;\n", nb_ligne + 3); //Il faut aller à l'instruction dans le TQ
    fprintf(exefile, "LOAD #0;\n");
    fprintf(exefile, "JUMP %d;\n", nb_ligne + 4);
    fprintf(exefile, "LOAD #1;\n");
    nb_ligne +=4;

    break;

  case '=': //OKAY
    codegen(p->op.noeud[1]); 
    
    fprintf(exefile,"INC %d;\n", RAM_OS_STK_REG); //RAM_OS_STK_REG incrémentation de la valeur de la pile à 1
    fprintf(exefile, "STORE @%d ;CODE OP\n\n", RAM_OS_STK_REG);

    codegen(p->op.noeud[0]);

    //Je commence par la soustraction
    fprintf(exefile, "SUB @%d; CODE DE =\n", RAM_OS_STK_REG);
    fprintf(exefile, "STORE @%d;\n", RAM_OS_STK_REG);
    fprintf(exefile, "INC %d; SOUSTRACTION EFFECTUÉ noeud[0] - noeud[1] = 0 ?\n\n", RAM_OS_STK_REG);
    
    nb_ligne += 5;
    fprintf(exefile, "JUMZ %d;\n", nb_ligne + 3); //Il faut aller à l'instruction dans le TQ
    fprintf(exefile, "LOAD #0;\n");
    fprintf(exefile, "JUMP %d;\n", nb_ligne + 4);
    fprintf(exefile, "LOAD #1;\n");
    nb_ligne +=4;
  
    break;
  
  default:
    break;
  }
}


/*
Pour savoir si une expression est:
= 0
> 0
< 0
if faut:
je note: r =  a < n 
r ici c'est notre EXP. Donc je choisi un des cas: =, >, ou <
sdonc :

r > 0 si: 
  n - a >  0
donc il faut faire une soustraction peut importe que ce soit <, >, ou =
*/
void codeSi(asa* p)
{
  //Partie exp
  codegen(p -> si.exp);

  codelen = parcours_asa(p -> si.si_instruct);
  fprintf(exefile, "JUMZ %d; EXP = 0 ?\n", nb_ligne + codelen); //je sors du TQ 
  nb_ligne +=1;

  //Partie instruct
  codegen(p -> si.si_instruct);
}



void codeTQ(asa* p)
{ 
  int save_nbligne = nb_ligne;
  //Partie exp
  codegen(p -> tq.exp);

  codelen = parcours_asa(p -> tq.instruct);
  fprintf(exefile, "JUMZ %d; EXP = 0 ?\n", nb_ligne + codelen); //je sors du TQ 
  nb_ligne +=1;

  //Partie instruct
  codegen(p -> tq.instruct);
  fprintf(exefile, "JUMP %d;\n\n", save_nbligne); //Ce jump est correcte
}



void codePour(asa* p)
{ 
  int save_nbligne = nb_ligne;
  //Partie exp
  codegen(p -> pour.exp);

  codelen = parcours_asa(p -> pour.instruct);
  fprintf(exefile, "JUMZ %d; EXP = 0 ?\n", nb_ligne + codelen); //je sors du TQ 
  nb_ligne +=1;

  //Partie instruct
  codegen(p -> pour.instruct);
  fprintf(exefile, "JUMP %d;\n\n", save_nbligne); //Ce jump est correcte
}


void codeID(asa* p)
{
  symbole* ele = ts_rechercher_identificateur(TABLE_SYMBOLES, p -> id.nom, CTXT);

  int adr_ram = ele -> adr;
 
  fprintf(exefile, "LOAD %d; CODE ID\n\n", adr_ram); //j'accède à l'adr ram réel 
  nb_ligne +=1;
}



//Génère le code en ram du Affect
/*
  ID <- EXP
  1° recup adresse ID, ACC =
  2° val de ID = EXP   LOAD
*/
void codeAffect(asa* p)
{
  //RÉCUPÈRE L'ADRESSE DE L'ID => partie de gauche de l'affect
  symbole* ele = ts_rechercher_identificateur(TABLE_SYMBOLES, p -> affect.gauche -> id.nom, CTXT);
  int adr_id = ele -> adr; //si le context est global on ajout + 32 car taille fixe

  //JE GÉNÈRE EN PREMIER LA PARTIE DROITE DE L'AFFECT
  codegen(p -> affect.droite);
  
  fprintf(exefile, "STORE %d; CODE AFFECT\n\n", adr_id);
  nb_ligne +=1;
}


/*
Une déclaration de variable c'est le stockage de la variable dans une 
adresse mémoire de la machine RAM 
1° Récupération de l'adresse de l'élé
2° Ajout de l'adresse RAM_OS_STK_REG (c'est l'adresse disponible
3° on ajout d'une valeur alétoire ici 0 
4° on stocke à l'adresse)
*/
void codeDecla(asa* p)
{
  symbole* ele = ts_rechercher_identificateur(TABLE_SYMBOLES, p -> decla.declaration -> id.nom, CTXT);
  int adr = ele -> adr;


  fprintf(exefile, "LOAD #0; CODE DECLA\n"); //Valeur de défaut
  fprintf(exefile, "STORE %d;\n\n", adr);
  nb_ligne +=2;

  codegen(p -> decla.decla_suivant);
}













