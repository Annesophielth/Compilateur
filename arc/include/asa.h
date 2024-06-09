/*
	CE FICHIER NOUS PERMET DE GÉNÉRER UN ARBRE ABSTRAIT À PARTIR DES FEUILLES
Il intéragi avec:
									°Parser:= construire la structure d'arbre dont les feuilles sont des terminaux
									°TSB:= Table de symbole:= tien à jour les ID avec leurs informations

C'est ici que nous créons tous les types de noueds existant.
Deux types de noeuds:
									-Feuille NB
									-Noeud OP

#================================
				!!! OPTION !!!
			-Coloration syntaxique


#================================
				!!! À CODER ICI !!!

typeAdresse: @ID
	struct noeudAdresse
	cree_noeudAdresse(char* nom)

typeTab: ID[EXP]
	struct noeudTAb
	cree_noeudTab(ID, EXP)

typeSup: EXP > EXP
	struct noeud_Sup
	cree_noeudOP_Sup(asa* a, asa* p)






*/

#ifndef ASA_H
#define ASA_H


#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/*#====================
			LES DEFINE:
#======================*/
	//pour les: COULEURS
#define  TXT_RED    "\x1b[31m"
#define  TXT_GREEN  "\x1b[32m"
#define  TXT_BLUE   "\x1b[34m"
#define  TXT_BOLD   "\x1b[1m"
#define  TXT_NULL   "\x1b[0m"

	//pour les TAILLES des champs
#define  MAX_SIZE_FIELD_NAME  12
#define  MAX_SIZE_FIELD_VAL   32

	//pour les TYPES
#define TYPE_ENTIER  'e'
#define TYPE_TABLEAU 't'
#define TYPE_PTR     'p'
#define TYPE_FCT     'f'


typedef enum {
typeNB,
typeOP,
typeID,
typeAffectation,
typeAffect,
typeDecla,
typeSi,
typePour,
typeTQ,
typeListInstruction,
typeMain,
typeListDecla,
typeAlgo,
typeRenvoyer,
typeLire,
typeEcrire,
} typeNoeud;



typedef struct
{
	struct asa* ele;
}noeudEcrire;

typedef struct
{
	struct asa* ele;
}noeudLire;



typedef struct
{
	struct asa* ele;
}noeudRenvoyer;

typedef struct
{
	struct asa* decla;
	struct asa* main;
}noeudAlgo;

typedef struct
{
	struct asa* decla;
	struct asa* suivant;
}noeudListDecla;

typedef struct
{
	struct asa* decla;
	struct asa* instruct;
	struct asa* re;
}noeudMain;


typedef struct
{
	struct asa* instruct;
	struct asa* suivant;
}noeudListInstruction;


typedef struct
{
	struct asa* exp;
	struct asa* instruct;
	struct asa* suivant;
}noeudTQ;

typedef struct
{
	struct asa* exp;
	struct asa* instruct;
	struct asa* suivant;
}noeudPour;


typedef struct
{
	struct asa* exp;
	struct asa* si_instruct;
	struct asa* suivant;
}noeudSi;


typedef struct
{
	struct asa* gauche;  //id:= de type asa pour stocker les infos de <var> dans TSB
	struct asa* droite; //droite:= de type asa pour stocker les infos à droite de l'opérateur
}noeudAffect;


//#DÉCLARATION:= VAR toto, titi, tutu etc...
typedef struct
{
	struct asa* declaration;
	struct asa* decla_suivant;
}noeudDeclaration; // !!! CHECK !!!

//#OPÉRATEUR
typedef struct
{
  int ope;
  struct asa* noeud[2]; //allocation pour deux structures de type asa
} noeudOp;



	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!! #==== les FEUILLES #==== !!!
//#IDENTIFICATEUR
typedef struct
{
	char nom[32]; //stockage du nom de l'ID
}feuilleID;


//#NOMBRE
typedef struct
{
  int val; //stockage de la valeur de NB
} feuilleNb;


//# STRUCTURE asa
typedef struct asa
{
  typeNoeud type;
  int memadr; //stockage de l'adresse de la valeur
  int codelen; //

  union
  {
  	feuilleID id;
    feuilleNb nb;
    noeudOp op;
    noeudAffect affect;
    noeudDeclaration decla;
    noeudSi si;
    noeudPour pour;
    noeudTQ tq;

    noeudListInstruction listinstruction;
    noeudMain main;
    noeudListDecla listdecla;
    noeudAlgo algo;
    noeudRenvoyer renvoyer;
    noeudLire lire;
    noeudEcrire ecrire;
  };
} asa;



/*#====================
			LES FONCTIONS:
#======================*/
	//CRÉATION DES NEOUDS
asa* creer_feuilleID(char* nom); 
asa * creer_feuilleNB(int value ); 
asa * creer_noeudOP(int ope, asa * p1, asa * p2 ); 
asa* creer_noeudAffect(asa* a, asa* b);
asa* creer_noeudDeclaration(asa* a, asa* b);
asa* creer_noeudListInstruction(asa* a, asa* b);
asa* creer_noeudListDecla(asa* a, asa* b);
asa* creer_noeudSi(asa* exp, asa* a, asa* b);
asa* creer_noeudPour(asa* exp, asa* a, asa* b);
asa* creer_noeudTQ(asa* exp, asa* a, asa* b);
asa* creer_noeudMain(asa* a, asa* b, asa* c);
asa* creer_noeudAlgo(asa* a, asa* b);
asa* creer_noeudRenvoyer(asa* a);
asa* creer_noeudEcrire(asa* a);
asa* creer_noeudLire(asa* a);

	//AFFICHAGES
void print_asa(asa * p);
void print_typeOP(asa *p); //OKAY
void print_typeNB(asa *p); //OKAY
void print_typeID(asa* p); //OKAY
void print_typeTab(asa* p);
void print_typeDecla(asa* p);//OKAY
void print_typeAffect(asa* p);//OKAY
void print_typeListInstruction(asa* p);
void print_typeSi(asa* p);
void print_typePour(asa* p);
void print_typeTQ(asa* p);
void print_typeMain(asa* p);
void print_typeListDecla(asa* p);
void print_typeAlgo(asa* p);
void print_typeRenvoyer(asa* p);
void print_typeEcrire(asa* p);
void print_typeLire(asa* p);

	//EN PLUS UTILE
void error_asa(const char * s);
void free_asa(asa *p);

#endif
