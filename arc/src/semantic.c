#include "semantic.h"


char CTXT[32] = CTXGLB;
int pile_ram = 6; //début des registres pour save les variables

void semantic(asa *p)
{
  if (!p) return;
  switch (p->type)
  {
		case typeNB:
			printf("AJOUT NB\n");
		  semantic_NB(p);
		  break;
		  
		case typeOP:
			printf("AJOUT OP\n");
		  semantic_OP(p);
			break;
			
		case typeID:
			printf("AJOUT ID\n");
			semantic_ID(p);
			break;

		case typeDecla:
			printf("AJOUT DÉCLARATION\n");
			semantic_Decla_var(p);
		  break;

		case typeAffect:
			printf("AJOUT AFFECT\n");
			semantic_Affect(p);
			break;

		case typeMain:
			printf("AJOUT MAIN\n");
			semantic_Main(p);
			break;

		case typeRenvoyer:
			printf("RENVOYER\n");
			semantic_Renvoyer(p);
			break;

		case typeListInstruction:
			printf("AJOUT LISTE INSTRUCT\n");
			semantic_ListInstruction(p);
			break;

		case typeListDecla:
			printf("AJOUT LISTE DÉCLA\n");
			semantic_ListDecla(p);
			break;

		case typeAlgo:
			printf("\n\t##### PARTIE SUR LE SÉMANTIC ####\n");
			printf("AJOUT ALGO\n");
			semantic_Algo(p);
			break;

		case typeSi:
			printf("AJOUT SI\n");
			semantic_Si(p);
			break;

    case typeLire:
    	printf("AJOUT LIRE\n");
    	semantic_Lire(p);
    	break;

    case typeEcrire:
    	printf("AJOUT ECRIRE\n");
    	semantic_Ecrire(p);
    	break;

    case typeTQ:
    	printf("AJOUT TQ\n");
    	semantic_TQ(p);
    	break;

    case typePour:
    	printf("AJOUT Pour\n");
    	semantic_Pour(p);
    	break;

		default:
		  break;
  }
}



void semantic_OP(asa * p)
{
  switch (p->op.ope)
  {
  case '+':  // operateur binaire associatif a gauche (+ - * / % )
    semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;

  case '-':
  	semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;

  case '*':
  	semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;

  case '/':
  	semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;

  case '%':	
  	semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;

  case '>':
  	semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;

  case '<':
  	semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;

  case '=':
    semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;
  }
}


void semantic_Lire(asa* p)
{
	if (ts_rechercher_identificateur(TABLE_SYMBOLES, p -> lire.ele -> id.nom, CTXT) == NULL)
	{
		fprintf(stderr,"\x1b[31m[Warning semantic_fonction]\x1b[0m LIRE \x1b[32m%s\x1b[0m pas déclaré\x1b[32m%s\x1b[0m\n", p -> lire.ele -> id.nom, CTXT);
		exit(1); //Erreur
	}
}



void semantic_Ecrire(asa* p)
{
	if (ts_rechercher_identificateur(TABLE_SYMBOLES, p -> ecrire.ele -> id.nom, CTXT) == NULL)
	{
		fprintf(stderr,"\x1b[31m[Warning semantic_fonction]\x1b[0m ECRIRE \x1b[32m%s\x1b[0m pas déclaré\x1b[32m%s\x1b[0m\n", p -> ecrire.ele -> id.nom, CTXT);
		exit(1); //Erreur
	}
}




/*
Ici je n'ai pas besoin de vérifier si déja dans un contexte 
car c'est la première fonction appelée
*/
void semantic_Algo(asa* p)
{
	if (p -> algo.decla != NULL)
		semantic(p -> algo.decla);

	semantic(p -> algo.main);
}



/*
Il faut que je vérifie si déjà dans le context ou pas 
S'il éxite envoier un message d'erreur et inversement
NULL = pas dans le contexte 
*/
void semantic_Renvoyer(asa* p)
{
	char* id = p -> renvoyer.ele -> id.nom;
	if (ts_rechercher_identificateur(TABLE_SYMBOLES, id, CTXT) == NULL) 
	{
		fprintf(stderr,"\x1b[31m[Warning semantic_fonction]\x1b[0m RENVOYER \x1b[32m%s\x1b[0m pas déclaré\x1b[32m%s\x1b[0m\n", id, CTXT);
		exit(1); //Erreur
	}
}




void semantic_ListInstruction(asa* p)
{
	if (p -> listinstruction.instruct != NULL)
		semantic(p -> listinstruction.instruct);
	
	if (p -> listinstruction.suivant != NULL)
		semantic(p -> listinstruction.suivant);
}


void semantic_ListDecla(asa* p)
{
	if (p ->listdecla.decla != NULL)
		semantic(p -> listdecla.decla);

	if (p ->listdecla.suivant != NULL)
		semantic(p -> listdecla.suivant);
}


void semantic_Main(asa* p)
{
	pile_ram +=1;
	
	if (ts_rechercher_identificateur(TABLE_SYMBOLES, "MAIN", CTXT) == NULL)
		ts_ajouter_identificateur(TABLE_SYMBOLES, CTXT, "MAIN", TYPE_FCT, 1);	


	if (ts_rechercher_contexte(TABLE_SYMBOLES, "MAIN") == NULL)
		ts_ajouter_contexte(TABLE_SYMBOLES, "MAIN");

	
	else
		fprintf(stderr,"\x1b[31m[Warning semantic_fonction]\x1b[0m MAIN \x1b[32m%s\x1b[0m déjà presente dans le contexte \x1b[32m%s\x1b[0m\n", "MAIN", CTXT);
	
	strcpy(CTXT, "MAIN");
	
	semantic(p -> main.decla);
	semantic(p -> main.instruct);

	strcpy(CTXT, "GLOBAL");
}


void semantic_NB(asa * p)
{
	NULL;
}


void semantic_ID(asa* p)
{
	if(ts_rechercher_identificateur(TABLE_SYMBOLES, p -> id.nom, CTXT) == NULL)
	{	
		fprintf(stderr,"\x1b[31m[Warning semantic_fonction]\x1b[0m ID \x1b[32m%s\x1b[0m pas déclarés \x1b[32m%s\x1b[0m\n", p -> id.nom, CTXT);
		exit(1);
	}
}


/*
Il n'y a qu'ici que j'ajoute l'id à la table de symbole 
Sinon ça aveut dire qu'il y a une erreur 
et que l'id n'est pas déclaré
donc arrêt du programme 
exit(1)
*/
void semantic_Decla_var(asa*p)
{
	if (p -> decla.declaration != NULL)
	{
	  char* id = p -> decla.declaration -> id.nom;
	  if(ts_rechercher_identificateur(TABLE_SYMBOLES, id, CTXT) == NULL)
	    ts_ajouter_identificateur(TABLE_SYMBOLES, CTXT, id, TYPE_ENTIER, 1);//sinon ajout 'id' avec context global

	 	symbole* ele = ts_rechercher_identificateur(TABLE_SYMBOLES, p -> decla.declaration -> id.nom, CTXT);
		ele -> adr = pile_ram;
		pile_ram += 1;
	}	

	semantic(p -> decla.decla_suivant);
}

void semantic_Affect(asa * p)
{
  char* id_gauche = p -> affect.gauche -> id.nom;
  if ( ts_rechercher_identificateur(TABLE_SYMBOLES, id_gauche, CTXT) == NULL)
 	{	
 		fprintf(stderr,"\x1b[31m[Warning semantic_fonction]\x1b[0m AFFECT \x1b[32m%s\x1b[0m  n'est pas déclaré \x1b[32m%s\x1b[0m\n", id_gauche, CTXT);
 		exit(1); //Arrêt du programme 
	}

	else
  	semantic(p -> affect.droite);
}

/*
Il faut vérifier si s'il à un id qu'il est bien déclaré
EXP du SI est forcément un noeud opérateur 
*/
void semantic_Si(asa* p)
{
	//Partie exp
	semantic(p -> si.exp); //S'il y a un id, doit nous dire s'il est déclaré ou pas

	//Partie instruction
	semantic(p -> si.si_instruct);
}

void semantic_TQ(asa* p)
{
	//Partie exp
	semantic(p -> tq.exp); //S'il y a un id, doit nous dire s'il est déclaré ou pas

	//Partie instruction
	semantic(p -> tq.instruct);
}

void semantic_Pour(asa* p)
{
	//Partie exp
	semantic(p -> pour.exp); //S'il y a un id, doit nous dire s'il est déclaré ou pas

	//Partie instruction
	semantic(p -> pour.instruct);
}

