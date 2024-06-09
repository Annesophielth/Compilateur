#include "asa.h"

static const char str_type[][16] = {
"NB", 
"OP",
"ID", 
"AFFECTATION", 
"AFFECT",
"DECLA", 
"SI",
"POUR",
"TQ",
"LISTINSTRUCT",
"MAIN",
"LISTDECLA",
"ALGO",
"RENVOYER",
"LIRE",
"ECRIRE"
};
static const char line[36] = "-----------------------------------";

static char buffer[32];
static char TABULATION[256] = "";
static int indent = 0;





asa* creer_noeudEcrire(asa* a)
{
  asa* p;
  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("échec allocation mémoire");

  p -> type = typeEcrire;
  p -> ecrire.ele = a;

  return p;
}



asa* creer_noeudLire(asa* a)
{
  asa* p;
  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("échec allocation mémoire");

  p -> type = typeLire;
  p -> lire.ele = a;

  return p;
}



asa* creer_noeudRenvoyer(asa* a)
{
  asa* p;
  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("échec allocation mémoire");

  p -> type = typeRenvoyer;
  p -> renvoyer.ele = a;

  return p;  
}


asa* creer_noeudListDecla(asa* a, asa* b)
{
  asa* p;
  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("échec allocation mémoire");


  p -> type = typeListDecla;
  p -> listdecla.decla = a;
  p -> listdecla.suivant = b;

  return p;
}

asa* creer_noeudMain(asa* a, asa* b, asa* c)
{
  asa* p;
  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("échec allocation mémoire");

  p -> type = typeMain;
  

  p -> main.decla = a;
  p -> main.instruct = b;
  p -> main.re = c;

  return p;
}


asa* creer_noeudAlgo(asa* a, asa* b)
{
  asa* p;
  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("échec allocation mémoire");

  p -> type = typeAlgo;
  
  if (a != NULL)
    p -> algo.decla = a;
  
  p -> algo.main = b;

  return p;
}


asa* creer_noeudListInstruction(asa* a, asa* b)
{
  asa* p;
  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("échec allocation mémoire");

  p -> type = typeListInstruction;
  p -> listinstruction.instruct = a;
  p -> listinstruction.suivant = b;

  return p;
}

asa* creer_noeudSi(asa* exp, asa* a, asa* b)
{
  asa* p;
  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("échec allocation mémoire");


  p -> type = typeSi;

  p -> si.exp = exp;
  p -> si.si_instruct = a;

  if (b != NULL)
    p -> si.suivant = b;

  return p;
}


asa* creer_noeudPour(asa* exp, asa* a, asa* b)
{
    asa* p;
  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("échec allocation mémoire");

  p -> type = typePour;

  p -> pour.exp = exp;
  p -> pour.instruct = a;

  if (b != NULL)
    p -> pour.suivant = b;

  return p;
}


asa* creer_noeudTQ(asa* exp, asa* a, asa* b)
{
    asa* p;
  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("échec allocation mémoire");

  p -> type = typeTQ;

  p -> tq.exp = exp;
  p -> tq.instruct = a;

  if (b != NULL)
    p -> tq.suivant = b;

  return p;
}



asa* creer_noeudAffect(asa* a, asa* b)
{
  asa* p;
  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("échec allocation mémoire");

  p -> type = typeAffect;
  p-> affect.gauche = a;
  p -> affect.droite = b;

  return p;
}



asa* creer_noeudDeclaration(asa* a, asa* b) // !!! CHECK !!!
{
	asa* p;
	if ((p = calloc(1, sizeof(asa))) == NULL)
		error_asa("échec allocation mémoire");

	p->type = typeDecla;
	p->decla.declaration = a;
	p->decla.decla_suivant = b;

	return p;
}


asa* creer_feuilleID(char* nom)
{
	asa* p;
	if ((p = calloc(1, sizeof(asa))) == NULL)
		error_asa("échec allocation mémoire");

	p->type = typeID;
	strcpy(p->id.nom, nom);

	return p;
}


asa * creer_feuilleNB(int val)
{
  asa *p;

  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeNB;
  p->nb.val = val;

  return p;
}

asa * creer_noeudOP( int ope, asa * p1, asa * p2)
{
  asa * p;

  if ((p = calloc(1, sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeOP;
  p->op.ope = ope;
  p->op.noeud[0]=p1;
  p->op.noeud[1]=p2;

  return p;
}


void free_asa(asa *p)
{
  if (!p) return;
  switch (p->type) {
  case typeOP:
    free_asa(p->op.noeud[0]);
    free_asa(p->op.noeud[1]);
    break;
  case typeNB:
  default: break;
  }
  free(p);
}

static void print_field(char * field_name, char * field_val, char *TABULATION, char *escape_seq)
{
  char buffer[128] = "";
  sprintf(buffer, "%s|%s%*s| ",
	  TABULATION,
	  field_name,
	  MAX_SIZE_FIELD_NAME-(int)strlen(field_name),"");
  printf("%s%s%s" TXT_NULL "%*s\n",
	 buffer,
	 escape_seq,
	 field_val,
	 MAX_SIZE_FIELD_VAL-MAX_SIZE_FIELD_NAME-(int)strlen(field_val),"|");
}

void print_asa(asa * p){
  int i;
  if (!p) return;
  for (i=0; i<indent; i++){ TABULATION[i] = '\t'; }
  TABULATION[i]='\0';

  printf("%s%s\n", TABULATION, line);
 
  sprintf(buffer, "%p", p);
  print_field("  noeud", buffer, TABULATION,TXT_BLUE TXT_BOLD);

  sprintf(buffer, "%s", str_type[p->type]);
  print_field("  typeNoeud", buffer, TABULATION,"");

  printf("%s%s\n" \
	 "%s|%*s|%*s|\n",TABULATION, line,
	 TABULATION, MAX_SIZE_FIELD_NAME,"",MAX_SIZE_FIELD_VAL-MAX_SIZE_FIELD_NAME,"");

  sprintf(buffer, "%d", p->memadr);
  print_field("adr mem", buffer, TABULATION,"");

  sprintf(buffer, "%d", p->codelen);
  print_field("taille code", buffer, TABULATION,"");

  switch (p->type)
  {
  case typeID:
  	print_typeID(p);
    break;

  case typeOP:
    print_typeOP(p);
    break;

  case typeNB:
    print_typeNB(p);
    break;

  case typeDecla:
  	print_typeDecla(p);
    break;

  case typeAffect:
  	print_typeAffect(p);
    break;

  case typeMain:
    print_typeMain(p);
    break;

  case typeRenvoyer:
    print_typeRenvoyer(p);
    break;


  case typeListInstruction:
    print_typeListInstruction(p);
    break;

  case typeListDecla:
    print_typeListDecla(p);
    break;

  case typeAlgo:
    //printf("ALGO\n");
    print_typeAlgo(p);
    break;

  case typeLire:
    print_typeLire(p);
    break;
  
  case typeEcrire:
    print_typeEcrire(p);
    break;

  case typeSi:
    //printf("SI\n");
    print_typeSi(p);
    break;

  case typePour:
    //printf("POUR\n");
    print_typePour(p);
    break;

  case typeTQ:
    //printf("TQ\n");
    print_typeTQ(p);
    break;

  default:
    break;
  }
}



void print_typeRenvoyer(asa* p)
{
  sprintf(buffer, "%p", p -> renvoyer.ele);
  print_field("RENVOYER", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);


  indent++; print_asa(p -> renvoyer.ele); indent--;
}


void print_typeOP(asa *p)
{
  sprintf(buffer, "'%c'", p -> op.ope);
  print_field("operateur", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  sprintf(buffer, "%p", p -> op.noeud[0]);
  print_field("ope gauche", buffer, TABULATION,TXT_GREEN);
  printf("%s%s\n", TABULATION,line);

  sprintf(buffer, "%p", p -> op.noeud[1]);
  print_field("ope droite", buffer, TABULATION,TXT_GREEN);
  printf("%s%s\n", TABULATION,line);


  indent++; print_asa(p -> op.noeud[0]); indent--;
  indent++; print_asa(p -> op.noeud[1]); indent--;
}

void print_typeNB(asa *p)
{
  sprintf(buffer, "%d", p->nb.val);
  print_field("val", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);
}


void print_typeID(asa* p)
{
  sprintf(buffer, "%s", p->id.nom);
  print_field("id", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);
}



void print_typeDecla(asa* p)
{
  sprintf(buffer, "%p", p -> decla.declaration	);
  print_field("id", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  sprintf(buffer, "%p", p -> decla.decla_suivant);
  print_field("decla suivante", buffer, TABULATION,TXT_GREEN);
  printf("%s%s\n", TABULATION,line);

  indent++; print_asa(p -> decla.declaration); indent--;
  indent++; print_asa(p -> decla.decla_suivant); indent--;

}



void print_typeAffect(asa* p)
{
  sprintf(buffer, "%p", p->affect.gauche);
  print_field("gauche", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);


  sprintf(buffer, "%p", p->affect.droite);
  print_field("droite", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);


  indent++; print_asa(p->affect.gauche); indent--;
  indent++; print_asa(p->affect.droite); indent--;

}


void print_typeSi(asa* p)
{
  sprintf(buffer, "%p", p -> si.exp);
  print_field("SI Exp", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);


  sprintf(buffer, "%p", p -> si.si_instruct);
  print_field("SI Instruc", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  if (p -> si.suivant != NULL)
  {
    sprintf(buffer, "%p", p -> si.suivant);
    print_field("Exp", buffer, TABULATION,"");
    printf("%s%s\n", TABULATION,line);
  }

  indent++; print_asa(p -> si.exp); indent--;
  indent++; print_asa(p -> si.si_instruct); indent--;

  if (p -> si.suivant != NULL)
  {
      indent++; print_asa(p -> si.suivant); indent--;
  }
}


void print_typePour(asa* p)
{
  sprintf(buffer, "%p", p -> pour.exp);
  print_field("Exp", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  sprintf(buffer, "%p", p -> pour.instruct);
  print_field("Instruction", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);


  if (p -> pour.suivant != NULL)
  {
    sprintf(buffer, "%p", p -> pour.suivant);
    print_field("Suivant", buffer, TABULATION,"");
    printf("%s%s\n", TABULATION,line);
  }

  indent++; print_asa(p -> pour.exp); indent--;
  indent++; print_asa(p -> pour.exp); indent--;

  if (p -> pour.suivant != NULL)
  {
    indent++; print_asa(p -> pour.exp); indent--;
  }

}


void print_typeTQ(asa* p)
{
  sprintf(buffer, "%p", p -> tq.exp);
  print_field("Exp", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  sprintf(buffer, "%p", p -> tq.instruct);
  print_field("Instruction", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);


  if (p -> tq.suivant != NULL)
  {
    sprintf(buffer, "%p", p -> tq.suivant);
    print_field("Suivant", buffer, TABULATION,"");
    printf("%s%s\n", TABULATION,line);
  }

  indent++; print_asa(p -> tq.exp); indent--;
  indent++; print_asa(p -> tq.instruct); indent--;

  if (p -> tq.suivant != NULL)
  {
    indent++; print_asa(p -> tq.exp); indent--;
  }

}


void print_typeListInstruction(asa* p)
{

  sprintf(buffer, "%p", p -> listinstruction.instruct);
  print_field("Instruction", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  sprintf(buffer, "%p", p -> listinstruction.suivant);
  print_field("Suivante", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);


  indent++; print_asa(p -> listinstruction.instruct); indent--;
  indent++; print_asa(p -> listinstruction.suivant); indent--;


}


void print_typeMain(asa* p)
{

  sprintf(buffer, "%p", p -> main.decla);
  print_field("DÉCLARATIONS", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  sprintf(buffer, "%p", p -> main.instruct);
  print_field("INSTRUCTIONS", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  sprintf(buffer, "%p", p -> main.re);
  print_field("RENVOYER", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);


  indent++; print_asa(p -> main.decla); indent--;
  indent++; print_asa(p -> main.instruct); indent--;
  indent++; print_asa(p -> main.re); indent--;

}

void print_typeListDecla(asa* p)
{

  sprintf(buffer, "%p", p -> listdecla.decla);
  print_field("Instructions", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  sprintf(buffer, "%p", p -> listdecla.suivant);
  print_field("Fonction main", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  indent++; print_asa(p -> listdecla.decla); indent--;
  indent++; print_asa(p -> listdecla.suivant); indent--;

}

void print_typeAlgo(asa* p)
{
  sprintf(buffer, "%p", p -> algo.decla);
  print_field("Instructions", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);


  sprintf(buffer, "%p", p -> algo.main);
  print_field("Fonction main", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  indent++; print_asa(p -> algo.decla); indent--;

  if (p -> algo.main != NULL)  
  {
    indent++; print_asa(p -> algo.main); indent--;
  }

}

void print_typeLire(asa* p)
{
  sprintf(buffer, "%p",p -> lire.ele);
  print_field("LIRE", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

  indent++; print_asa(p -> lire.ele); indent--;
}



void print_typeEcrire(asa* p)
{
  sprintf(buffer, "%p", p -> ecrire.ele);
  print_field("ECRIRE", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);
  
  indent++; print_asa(p -> ecrire.ele); indent--;
}


void error_asa(const char * s){
  fprintf(stderr, TXT_BOLD TXT_RED "[erreur ASA]" TXT_NULL " %s", s);
  exit(1);
}
