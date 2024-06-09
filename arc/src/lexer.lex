%{
  #include <string.h>
  #include "parser.h"

  extern void yyerror(char *);

  char errmsg[256]="";
  const char charerr[] = "\x1b[1m\x1b[31m[erreur lexicale]\x1b[0m caractère \x1b[41m%c\x1b[0m inattendu";
  
  /* MACRO défini 
   * Action executee avant chaque action semantique (meme vide)  
   * et rempli la variable Bison `yylloc` avec la position du token
   */
#define YY_USER_ACTION                                             \
  yylloc.first_line = yylloc.last_line;                            \
  yylloc.first_column = yylloc.last_column;                        \
  if (yylloc.last_line == yylineno)                                \
    yylloc.last_column += yyleng;                                  \
  else                                                             \
  {                                                                \
    yylloc.last_line = yylineno;                                   \
    yylloc.last_column = 1;					                               \
  }



%}
  
%option nounput
%option noinput
%option yylineno

CHIFFRE  [0-9]
NOMBRE   [0-9]+
IDENT    [a-zA-Z]+
%%

{NOMBRE}        {yylval.nb = atoi(yytext); return NB; }
[-*+/=%\n(),@><\[\]]  {return *yytext;}
"PROGRAMME"     {return PROGRAMME;}
"DEBUT"         {return DEBUT;}
"ALGO"					{return ALGO;}
"RENVOYER"			{return RENVOYER;}
"<-"            {return AFFECT;}
"FIN"           {return FIN;}
"VAR"           {return VAR;}
"SI" 						{return SI;}
"ALORS"         {return ALORS;}
"FSI"           {return FSI;}
"FTQ" 					{return FTQ;}
"SINON"					{return SINON;}
"TQ"						{return TQ;}
"FAIRE"					{return FAIRE;}
"LIRE"          {return LIRE;}
"ECRIRE"        {return ECRIRE;}
"POUR"				  {return POUR;}
"FPOUR" 				{return FPOUR;}
[ \t]           { /* ignorer les blancs */ }
{IDENT}         {strcpy(yylval.id, yytext); return ID;}
.         {           
	    sprintf(errmsg,charerr, yytext[0]);
            yyerror(errmsg);
            return 1;
          }

%%
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
