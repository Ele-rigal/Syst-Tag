#include "tag.h"
#include "parser.h"
#include "interpreter.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>


//renvoie vrai si le chemin est valide
int valid_path(char *path, struct stat sb){
  if(lstat(path,&sb)==-1)
    return FALSE;
  else
    return TRUE;
}
//verifie le type des éléments
int type_element(char *path){
  struct stat sb;
  if(valid_path(path,sb)==FALSE){
    perror("path invalide");
    exit(1);
  }
  lstat(path,&sb);
  printf("Type de fichier :    ");
  switch (sb.st_mode & S_IFMT) {
    case S_IFDIR:  printf("répertoire\n"); return 1;
    case S_IFLNK:  printf("lien symbolique\n"); return 2;
    case S_IFREG:  printf("fichier ordinaire\n"); return 3;
    default:  printf("inconnu ?\n"); return -1;
  }
}

//retourn la taille des attributs:
int len_attribut(char *path){
  int len=getxattr(path,"user.tag",0,0);
	if(len==-1)
		return 0;
	else
		return len;

}

//liste les attributs de tous les attributs d'un répertoire
void liste_all(char *directory){
	DIR *rep=NULL;
	struct dirent* fd=NULL;
	rep=opendir(directory);
	if(rep==NULL){
		perror("bad path");
		exit(1);
	}
	char *file;
	printf("rep ouvert succes\n");
	while((fd=readdir(rep)) !=NULL){
		file=fd->d_name;
		read_attribut(file);
	}
	printf("\n\n");
	if(closedir(rep) ==-1)
		exit(1);
}

//cherche si un tag apparait dans le tableau d'id d'un fichier
int search_tag_id(int *t,int len,char *tag){
	int i;
	if(t==NULL)
		return FALSE;
	for(i=0; i<len; i++){
		//printf("i : %d\n",i);
		if(strcmp(get_tag(t[i]),tag)==0){
			//printf("get_tag id :  %s  : tag :%s",get_tag(t[i]),tag);
			return TRUE;
		}
		int *id=check_hierarchie(get_tag(t[i]));
		return(search_tag_id(id,sizeof(id),tag));
		
	}	
	return FALSE;
}

//renvoie tous les fichiers qui ne sont pas de ce tags
void search_tag_not(char *path,char *tags){
	DIR *rep=NULL;
	struct dirent* fd=NULL;
	rep=opendir(path);
	int len;
	if(rep==NULL){
		perror("bad path");
		exit(1);
	}
	char *file;
	printf("rep ouvert succes\n");
	while((fd=readdir(rep)) !=NULL){
		file=fd->d_name;
		int *t;
		if((t=get_attribut(file))==NULL)
			printf("\nOK : %s\n",file);
		else{	
			len=len_attribut(file)/sizeof(int);
			if(search_tag_id(t,len,tags)==FALSE){
				printf("\nOK : %s\n",file);
			}
		}
	
	}
	printf("\n\n");
	if(closedir(rep) ==-1)
		exit(1);
}
		
//cherche les fichiers possédant ce tag dans un répertoire 
void search_tag_dir(char *path,char *tag){
	DIR *rep=NULL;
	struct dirent* fd=NULL;
	rep=opendir(path);
	int len;
	if(rep==NULL){
		perror("bad path");
		exit(1);
	}
	char *file;
	printf("rep ouvert succes\n");
	printf("vous avez demandé ce tag : %s\n",tag);
	while((fd=readdir(rep)) !=NULL){
		file=fd->d_name;
		int *t;
		if((t=get_attribut(file))==NULL){		
			//printf("n'a pas d'attribut : %s\n",file);
		}
		else{	
			//printf("len : %d\n",len);
			len=len_attribut(file)/sizeof(int);
			if(search_tag_id(t,len,tag)==TRUE){
				//printf("yeah\n");
				printf("\nOK : %s\n",file);
			}
		//printf("%s\n",file);
		}
	
	}
	printf("\n\n");
	if(closedir(rep) ==-1)
		exit(1);

}
//renvoie les attributs
int *get_attribut(char *path){
	int len=len_attribut(path)/sizeof(int);
	int *t;
	//printf("\n%10s",path);
	t=malloc(len*sizeof(int));
	if(getxattr(path,"user.tag",(void *)t,len*sizeof(*t))<0) 
		return NULL;
	return t;
}	


//print les attributs 
void read_attribut(char *path){
  int len=len_attribut(path)/sizeof(int);
 if(len==0)
	return;
  int *t;
  printf("\n%10s",path);
  t=malloc(len*sizeof(int));
  if (getxattr(path,"user.tag",(void *)t,len*sizeof(*t))<0)
	return;

  for(int i=0; i<len; i++){
    printf(" %d : ",t[i]);
    printf("%5s \n",get_tag(t[i]));
  }
}

		

//dit si un tag existe déjà dans user.tag
int exist_tag(int *t,int id,int len) {
	int i;
	for(i=0; i<len; i++){
		if(id==t[i])
			return TRUE;
	}
	return FALSE;
}

//parse mes arguments
int parse_tag(const char *tag){
	char *tok;
	char *rest=strdup(tag);
	char *last_tag=NULL;
	int id;
	while((tok = strtok_r(rest,"/",&rest))!=NULL){ 
		id=new_id();
		id=exist_tag_json((char*)tok,id);
		if(last_tag)
			ajout_hierarchie(id,last_tag);
		//printf("%s \n",tok);
		last_tag=tok;
	}
	return id;
}


//renvoie le tableau des ids qu'il faut trouver dans les fichiers - ceux qu'il ne faut pas
int *id_envoie(char **argv,int len,int *taille1){
	int i;
	int *ids;
	int *not;
	not=malloc(sizeof(int));
	ids=malloc(sizeof(int));
	
	int taille2=0;
	//printf("blup\n");
	//printf("len: %d\n",len);
	i=0;
	if(strcmp(argv[0],"ET")==0){
		//printf("début\n");
		printf("mal écrit");
		exit(0);
	}
	while(i<len){
		//printf("i : %d\n",i);
		if(strcmp(argv[i],"PAS")==0){
			if(i+1>=len)
				break;
			//printf("pas : %s\n",argv[i+1]);
			not[taille2]=parse_tag(argv[i+1]);
			taille2++;
			not=realloc(not,taille2*sizeof(int));
			i+=2;
		}
		else if(strcmp(argv[i],"ET")!=0){
			//printf("%s ",argv[i]);
			if(i+1<len){
				if(strcmp(argv[i+1],"ET")==0){
					//printf("et ");
					if(i+2<len){
						if(strcmp(argv[i+2],"PAS")==0){
							if(i+3<len){
								if((strcmp(argv[i+3],"ET")==0) || (strcmp(argv[i+3],"PAS")==0)){
									printf("mal écrit");
									exit(0);
								}
								else{
									//printf("pas %s\n",argv[i+3]);
									not[taille2]=parse_tag(argv[i+3]);
									taille2++;
									not=realloc(not,taille2*sizeof(int));
									ids[*taille1]=parse_tag(argv[i]);
									*taille1=*taille1+1;
									ids=realloc(ids,(*taille1)*sizeof(int));
									i+=4;
								}
							}
							else break;
						}
						else if(strcmp(argv[i+2],"ET")==0){
							printf("mal écrit");
							exit(0);
						}
						else{
							if(i+3<len){
								if(strcmp(argv[i+3],"ET")!=0 ){
									printf("mal écrit");
									exit(0);
								}
							}
							//printf("%s \n",argv[i+2]);
							*taille1=*taille1+2;
							ids=realloc(ids,(*taille1)*sizeof(int));
							ids[*taille1-2]=parse_tag(argv[i]);
							ids[*taille1-1]=parse_tag(argv[i+2]);
							i+=3;
						}
					}
					else break;
				}
				else{
					printf("mal écrit");
					exit(0);
				}
			}
			else break;
		}
		else if(strcmp(argv[i],"ET")==0){
			//printf("et ");
			if(i+1<len){
				if(strcmp(argv[i+1],"PAS")==0){
					if(i+2<len){
						if((strcmp(argv[i+2],"ET")==0) || (strcmp(argv[i+2],"PAS")==0)){
							printf("mal écrit");
							exit(0);
						}
						else{
							if(i+3<len){
								if(strcmp(argv[i+3],"ET")!=0){
									printf("mal écrit");
									exit(0);
								}
							}
							//printf("pas %s\n",argv[i+2]);
							not[taille2]=parse_tag(argv[i+2]);
							taille2++;
							not=realloc(not,taille2*sizeof(int));
							i+=3;
						}
					}
					else break;
				}
				else if(strcmp(argv[i+1],"ET")!=0){
					if(i+2<len){
						if(strcmp(argv[i+2],"ET")!=0){
							printf("mal écrit");
							exit(0);
						}
					}
					//printf("%s \n",argv[i+1]);
					ids[*taille1]=parse_tag(argv[i+1]);
					*taille1=*taille1+1;
					ids=realloc(ids,(*taille1)*sizeof(int));
					i+=2;
				}
				else{
					printf("mal écrit");
					exit(0);
				}
			}
			else break;
		}
		else{
			printf("mal écrit");
			exit(0);
		}	
	}
	/*printf("tout: \n");
	for(i=0; i<*taille1; i++){
		printf("%d\n",ids[i]);
	}
	printf("non\n");
	for(i=0; i<taille2; i++){
		printf("%d\n",not[i]);
	}*/
	//int *len_y=&taille1;
	int *t=is_a_sub(ids,taille1,not,taille2);
	/*printf("final: \n");
	if(t==NULL)
		return NULL;
	for(i=0; i<*taille1; i++){
		printf("%d\n",t[i]);
	}*/
	int *blup;
	blup=malloc((*taille1)*sizeof(int));
	memcpy(blup,t,(*taille1)*sizeof(int));
	free(not);
	free(ids);
	return blup;
}	

int not_sub(int *t,int len,int id){
	int i;
	for(i=0; i<len; i++){
		if(sub(id,t[i])==TRUE)
			return FALSE;
	}
	return TRUE;
}
int id_ok(int *t1,int len1,int x){
	int i;
	for(i=0; i<len1; i++) {
		//printf("t[i]: %d",t1[i]);
		if(t1[i]==x)
			return TRUE;
	}
	return FALSE;
}		
//trouve un fichier en fonction des suites logiques entrées
void find_t(char *path, char **argv,int len){
	int taille1=0;
	int *t=id_envoie(argv,len,&taille1);
	int i;
	DIR *rep=NULL;
	struct dirent* fd=NULL;
	rep=opendir(path);
	int len1;
	if(rep==NULL){
		perror("bad path");
		exit(1);
	}
	char *file;
	//printf("%d taille\n",taille1);
	//printf("rep ouvert succes\n");
	while((fd=readdir(rep)) !=NULL){
		//printf("dans le while\n");
		file=fd->d_name;
		int *t2;
		if((t2=get_attribut(file))==NULL){		
			//printf("n'a pas d'attribut : %s\n",file);
		}
		len1=len_attribut(file)/sizeof(int);
		for(i=0; i<len1; i++){
			//printf("id: %d",t2[i]);
			if(id_ok(t,taille1,t2[i])==TRUE || (not_sub(t,taille1,t2[i]==FALSE))) 
				printf("%s \n",fd->d_name);
		}
	
	}
	printf("\n\n");
	if(closedir(rep) ==-1)
		exit(1);

}
	

		

//void read_tag(const char *tag){
void add_tag(char *path,const char *tag){
	int type=type_element(path);
	int len;
	int id=parse_tag(tag);
	/*int id=new_id();
	exist_tag_json((char*)tag,id);*/
	if(type==-1)
		exit(1);
 	else {
		len=len_attribut(path)/sizeof(int);
		//printf("len: %d \n",len);
		int *old_tags=get_attribut(path);
		if(exist_tag(old_tags,id,len)==TRUE)
			return;
		int tags[len+1];
		memcpy(tags,old_tags,len*sizeof(int));
		free(old_tags);
		tags[len]=id;
		if(setxattr(path,"user.tag",tags,(len+1)*sizeof(int),0)<0)
			perror("erreur setxattr");
		len=len_attribut(path)/sizeof(int);
		printf("tag ajouté\n");
	}
	/*
	c'etait dans le cas ù on faisait différement pour les liens symboliques:
	else {
		len=len_attribut_l(path)/sizeof(int);
		printf("len: %d \n",len);
		int *old_tags=read_attribut_l(path);
		if(exist_tag(old_tags,id,len)==TRUE)
			return;
		int tags[len+1];
	        memcpy(tags,old_tags,len*sizeof(int));
		free(old_tags);
		tags[len]=id;
		if(lsetxattr(path,"user.tag",tags,(len+1)*sizeof(int),0)<0)
			perror("erreur setxattr");
		len=len_attribut_l(path)/sizeof(int);
		printf("len: %d \n",len);
	}*/
}

void rm_tag(char *path, const char *tag) {
  printf("on entre dans rm tag\n");
  int len;
  int type = type_element(path);
  int id = parse_tag(tag);
  printf( " type = %d // id = %d\n",  type, id);
  
    if (type == -1) 
        return; 
    
    else {
        len = len_attribut(path)/sizeof(int);
        // printf("len = %d\n", len);
        int *tags = get_attribut(path);
        
        if (! exist_tag(tags, id, len))
            return;
        
        for (int i=0; i<len; i++) {
            if (tags[i] == id) {
                /*for (int j=i; j<len-1; j++) {
                    memcpy(tags[j], tags[j+1], sizeof(int));*/
		printf("id : %d\n",tags[i]);
		printf("id: %d \n",id);
		memcpy(tags+i,tags+i+1,(len-i)*sizeof(int));
		break;
                }
            }
        
        if (setxattr(path, "user.tag", tags, (len-1)*sizeof(int), 0)<0)
            perror ("erreur setxattr");
        free(tags);    
        len = len_attribut(path)/sizeof(int);
        printf("tag retiré\n");
        
    }
}
			
