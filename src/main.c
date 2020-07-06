#include "tag.h"
#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void help(char *prog){
	printf("Usage: %s <commande> [options]\n",prog);
	printf("\nCommande:\n");
	//printf("  - install	                        : pour installer à la première utilisation \n"); 
	printf("  - add <file> <tag>                    : pour ajouter un tag\n");
	printf("  - lt <file>               : pour lister les tags d'un fichier \n");
	printf("  - rmt <file> <tag>                    : pour supprimer le tag d'un fichier\n");
	printf("  - search <directory> <tag>            : pour chercher tous les fichiers possédant ce tag dans la liste \n");
	printf("  - search <directory> [logique] <tag>  : comme search mais avec logique(ET,PAS)\n");
	printf("\nOPTIONS:\n");
	printf(" -a :\n");
	printf("lt <directory> -a                       : pour lister tous les tags de tous les fichiers du répertoire\n");
	printf("\nLOGIQUE:\n");
	printf(" ET	                                : connecteur logique intersection & ; ∧\n");
	printf(" PAS                                    : connecteur logique negation ! ; ¬ \n");       
	printf("\nTag:\n");
	printf(" <tag> ou <tag/subtags/etc>             : simple tag ou hierarchie de tag\n");
	printf("\nHELP: \n");
	printf(" --help\n");
	exit(1);
}

void interprete_add(char **argv){
	add_tag(argv[0],argv[1]);
}
	
	

int main(int argc,char **argv){
	if(argc<2)
		help(argv[0]);
	else if(strcmp(argv[1],"install")==0){
		create_json();
	}
	else if(strcmp(argv[1],"add")==0) {
		if(argc<4)
			help(argv[0]);
		interprete_add(&argv[2]);
	}
	else if(strcmp(argv[1],"lt")==0){
		if(argc<3)
			help(argv[0]);
		else if(argc==3)
			read_attribut(argv[2]);
		else if(strcmp(argv[3],"-a")==0){
			liste_all(argv[2]);
		}
		else
			help(argv[0]);
	}
	else if(strncmp(argv[1],"rmt", 3)==0){
		if(argc<4)
			help(argv[0]);
		rm_tag(argv[2],(const char*)argv[3]);
	}
	else if(strcmp(argv[1],"search")==0){
		if(argc<4)
			help(argv[0]);
		else if(argc<5)
			search_tag_dir(argv[2],argv[3]);
		/*else if((strcmp(argv[3],"PAS")==0)&&(argc<6))
			search_tag_not(argv[2],argv[4]);*/
		else{
			printf("dans le main\n");
			find_t(argv[2],&argv[3],argc-3);
		}

	}	
	else if(strcmp(argv[1],"--help")==0){
		help(argv[0]);
	}
	else 
		help(argv[0]);
		
		
}
	
