%{
  #include <stdio.h>
  #include <ctype.h>
  #include <string.h>
  
  #include "asa.h"
  #include "semantic.h"
  #include "codegen.h"
  
  
  extern int yylex();
  static void print_file_error(char * s, char *errmsg);

  table_symb TABLE_SYMBOLES = NULL;
  struct asa * ARBRE_ABSTRAIT = NULL;
  
  
  void yyerror(const char * s);

  char srcname[64];
  char exename[64] = "a.out";
  FILE * exefile;
%}

%union
{
  int nb;
	struct asa* tree;  
	char id[32];
};

%define parse.error verbose
%locations




//LES TOKENS
%token <nb> NB
%token FIN
%token DEBUT
%token VAR
%token <id> ID
%token PROGRAMME
%token ALGO
%token AFFECT "<-"
%token SI 
%token SINON 
%token ALORS 
%token POUR 
%token TQ 
%token FAIRE
%token FSI
%token FTQ
%token FPOUR
%token ECRIRE
%token LIRE
%token RENVOYER



%type <tree> MAIN
%type <tree> EXP
%type <tree> INSTRUCTIONS
%type <tree> AFFECTATION
%type <tree> LIST_INSTRUCTIONS
%type <tree> LIST_DECLARATIONS 
%type <tree> LIST_ID
%type <tree> DECLARATIONS
%type <tree> SI_ALORS
%type <tree> BOUCLE_TQ
%type <tree> BOUCLE_POUR
%type <tree> PROGRAMME_ALGO


%right AFFECT 
%left '+' '-' '=' 
%left '*' '/' '%' '<' '>'



%start PROGRAMME_ALGO

%%
SEP: '\n' 
| SEP '\n'
;


PROGRAMME_ALGO:
	LIST_DECLARATIONS
	MAIN {$$ = creer_noeudAlgo($1, $2); ARBRE_ABSTRAIT = $$;}
;	
	
	
MAIN:
	PROGRAMME '(' ')' SEP
	DECLARATIONS
	DEBUT SEP
	LIST_INSTRUCTIONS
	FIN SEP {$$ = creer_noeudMain($5, $8, NULL);}
;




INSTRUCTIONS:
AFFECTATION  {$$ = $1;}
|EXP SEP 		 {$$ = $1;}
;


AFFECTATION:
ID "<-" EXP SEP 				{$$ = creer_noeudAffect(creer_feuilleID($1), $3);}
|ID "<-" AFFECTATION		{$$ = creer_noeudAffect(creer_feuilleID($1), $3);}
;



DECLARATIONS:
VAR LIST_ID SEP {$$ = $2;}
;



LIST_INSTRUCTIONS:
%empty 															{$$ = NULL;}
|INSTRUCTIONS LIST_INSTRUCTIONS 		{$$ = creer_noeudListInstruction($1, $2);}
|SI_ALORS LIST_INSTRUCTIONS 				{$$ = creer_noeudListInstruction($1, $2);}
|BOUCLE_TQ LIST_INSTRUCTIONS 				{$$ = creer_noeudListInstruction($1, $2);}
|BOUCLE_POUR LIST_INSTRUCTIONS 				{$$ = creer_noeudListInstruction($1, $2);}
|LIRE '(' EXP ')' SEP LIST_INSTRUCTIONS {$$ = creer_noeudListInstruction(creer_noeudLire($3), $6);}
|ECRIRE '(' EXP ')' SEP LIST_INSTRUCTIONS {$$ = creer_noeudListInstruction(creer_noeudEcrire($3), $6);}
|RENVOYER EXP SEP  {$$ = creer_noeudRenvoyer($2);}
;



LIST_DECLARATIONS:
%empty {$$ = NULL;}
|DECLARATIONS LIST_DECLARATIONS 			{$$ = creer_noeudListDecla($1, $2);}
;



LIST_ID:
ID							{$$ = creer_noeudDeclaration(creer_feuilleID($1), NULL);}
|ID ',' LIST_ID {$$ = creer_noeudDeclaration(creer_feuilleID($1), $3);}
;



SI_ALORS:
	SI EXP ALORS SEP
		LIST_INSTRUCTIONS  
	FSI		SEP		{$$ = creer_noeudSi($2, $5, NULL);}
;


BOUCLE_TQ:
	TQ EXP FAIRE SEP
		LIST_INSTRUCTIONS 
	FTQ SEP 			{$$ = creer_noeudTQ($2, $5, NULL);}
;



BOUCLE_POUR:
	POUR EXP FAIRE SEP
		LIST_INSTRUCTIONS 
	FPOUR SEP 			{$$ = creer_noeudPour($2, $5, NULL);}
;


EXP : 
NB                       {$$ = creer_feuilleNB($1);}
| ID                     {$$ = creer_feuilleID($1);}
| EXP '+' EXP            {$$ = creer_noeudOP('+',$1,$3);}
| EXP '-' EXP            {$$ = creer_noeudOP('-',$1,$3);}
| EXP '*' EXP            {$$ = creer_noeudOP('*',$1,$3);}
| EXP '/' EXP            {$$ = creer_noeudOP('/',$1,$3);}
| EXP '%' EXP            {$$ = creer_noeudOP('%',$1,$3);}
| EXP '=' EXP     			 {$$ = creer_noeudOP('=', $1, $3);} 
| EXP '>' EXP     			 {$$ = creer_noeudOP('>', $1, $3);} 
| EXP '<' EXP     			 {$$ = creer_noeudOP('<', $1, $3);} 
| '('EXP')'              {$$ = $2;}
;





%%



/*#====================
IL EST LA MON MAIN
#======================*/
int main( int argc, char * argv[] ) {
  extern FILE *yyin;
  TABLE_SYMBOLES = ts_init_table(CTXGLB);
  
  
  if (argc > 1){
    strcpy(srcname, argv[1]);
    if ( (yyin = fopen(srcname,"r"))==NULL ){
      char errmsg[256];
      sprintf(errmsg,"fichier \x1b[1m\x1b[33m' %s '\x1b[0m introuvable",srcname);
      print_file_error(argv[0],errmsg);
      exit(1);
    }
  }  else {
    print_file_error(argv[0],"aucun fichier en entree");
    exit(1);
  }
  if (argc == 3){
    strcpy(exename, argv[2]);
  }
  exefile = fopen(exename,"w");
  yyparse();

 	print_asa(ARBRE_ABSTRAIT); //Affichage de la l'arbre de symboles
 	semantic(ARBRE_ABSTRAIT); 	
  //ts_print(TABLE_SYMBOLES);
  codegen(ARBRE_ABSTRAIT);
  
  fprintf(exefile, "STOP;\n");
  

	

 	
  fclose(yyin);
}



static void print_file_error(char * prog, char *errmsg){
  fprintf(stderr,
	  "\x1b[1m%s:\x1b[0m \x1b[31m\x1b[1merreur fatale:\x1b[0m %s\nechec de la compilation\n",
	  prog, errmsg);
}

void yyerror(const char * s)
{
  fprintf(stderr, "\x1b[1m%s:%d:%d:\x1b[0m %s\n", srcname, yylloc.first_line, yylloc.first_column, s);
  exit(0);
}












