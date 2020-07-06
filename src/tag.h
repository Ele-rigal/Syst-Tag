#include <stdlib.h>
#include <sys/types.h>
#include <sys/xattr.h>
//#include <sys/nvpair.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>

#define repertoire 1
#define lien 2
#define fichier_ord 3
#define TRUE 1
#define FALSE 0
void add_tag(char *path,const char *tag);

void read_attribut(char *path);

int *get_attribut(char *path);

//void liste_file(char *tag,char *directory);

void liste_all(char *directory);

void search_tag_dir(char *path,char *tag);

void search_tag_not(char *path,char *tags);

void rm_tag(char *path, const char *tag);


void find_t(char *path,char **argv,int len);
