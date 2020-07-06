#include "interpreter.h"
#include "parser.h"
//#include "tag.h"
#include <sys/stat.h>
#include <json-c/json.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

//installation de la base de donnnées
void create_json(){
	int fd=open(CONF_PATH, O_WRONLY | O_CREAT,0744);
	if(fd<0)
		perror("erreur open");
	json_object *jobj=json_object_new_object();	
	json_object *root=json_object_new_object();
	json_object_object_add(jobj,"tags",root);
	size_t len=strlen(json_object_to_json_string(jobj));
	write(fd,json_object_to_json_string(jobj),len);
	close(fd);
}

//lit le fichier json
char *getfile(char *name)
{
    ssize_t    filelen;
    char    *content;
    int        fd;

    if ((fd = open(name, O_RDONLY)) == -1)
        return NULL;
    if ((filelen = lseek(fd, 0, SEEK_END)) < 1 || lseek(fd, 0, SEEK_SET) != 0)
        return NULL;
    if (!(content =calloc(filelen+1, sizeof(*content))))
        return (NULL);
    if (read(fd, content, filelen) != filelen)
        return (NULL);
    close(fd);
    return (content);
}

//lit mon fichier json
json_object *read_json(){
	char *str=getfile(CONF_PATH);
	//printf("%s \n\n",str);
	json_object * jobj =json_tokener_parse(str);
	//json_parse(jobj);
	return jobj;
}

//ecrit mes données de add_tag dans mon json
void write_json(const char *tag,int i,json_object *jobj){
	json_object *root=json_object_object_get(jobj,"tags");
	json_object *t=json_object_new_object();
	json_object *id=json_object_new_int(i);
	json_object *subtags=json_object_new_array();
	json_object_object_add(root,tag,t);
	json_object_object_add(t,"id",id);
	json_object_object_add(t,"subtags",subtags);
	int fd=open(CONF_PATH,O_WRONLY | O_TRUNC,0744);
	if(fd<0)
		perror("erreur open");
	struct stat sb;
	if(stat(CONF_PATH, &sb)<0)
		perror("erreur stat");
	write(fd,json_object_to_json_string(jobj),strlen(json_object_to_json_string(jobj)));
	close(fd);
}

//regarde si un Id existe dans un tableau
int id_in(int id,struct json_object *t){
	int i;
	int len=json_object_array_length(t);
	for(i=0; i<len; i++){
		if(id==json_object_get_int(json_object_array_get_idx(t,i)))
			return TRUE;
	}
	return FALSE;
}


//permet de renvoyer la hierarchie d'un tag 
int *check_hierarchie(char *tag){	
	struct json_object_iterator it;
 	struct json_object_iterator itEnd;
	struct json_object* obj;
 	obj =read_json();
	json_object *root=json_object_object_get(obj,"tags");
	json_object* j_tag=json_object_object_get(root,tag);
	json_object *obj_Id = json_object_object_get(j_tag,"id");
	json_object *obj_sub ;
	int id=json_object_get_int(obj_Id);
	//int len=json_object_object_length(root);
	int i=0;
	int taille=0;
	int *t;
	const char *tags;
	t=malloc(sizeof(int));
 	it = json_object_iter_begin(root);
 	itEnd = json_object_iter_end(root);
	json_object *check_tag;
	//printf("---------------------------------------------\n");
 	while (!json_object_iter_equal(&it, &itEnd)) {  			
		tags=json_object_iter_peek_name(&it);
		//printf("%s\n",tags);
		if(strcmp(tags,tag)!=0){
			check_tag=json_object_object_get(root,tags);
			obj_sub=json_object_object_get(check_tag,"subtags");
			if(id_in(id,obj_sub)==TRUE){
				taille++;
				t=realloc(t,taille*sizeof(int));
				t[taille-1]=json_object_get_int(json_object_object_get(check_tag,"id"));
			}	
		}
     		json_object_iter_next(&it);
		i++;
	 }
	/*printf("%s appartient à : \n",tag);
	for(i = 0; i<taille; i++){
		printf(" %d :: ",t[i]);
	}*/
	if(taille==0)
		return NULL;
	return t;
	printf("\n");
}
	
//renvoie un nouvel id 
int new_id() {
	struct json_object_iterator it;
 	struct json_object_iterator itEnd;
	struct json_object* obj;
 	obj =read_json();
	json_object *root=json_object_object_get(obj,"tags");
	int id=-1;
	json_object *check_tag;
	const char *tags;
	int len=json_object_object_length(root);
	//printf("len_json : %d\n",len);
	if(len==0)
		return 0;
	it = json_object_iter_begin(root);
 	itEnd = json_object_iter_end(root);
	while (!json_object_iter_equal(&it, &itEnd)) { 
		tags=json_object_iter_peek_name(&it);
		//printf("%s\n",tags);
		check_tag=json_object_object_get(root,tags);
		//printf("id :%d\n",id);
		id=json_object_get_int(json_object_object_get(check_tag,"id"));
		json_object_iter_next(&it);
	}
	id++;
	return id;
}

//ajoute un id dans les subtags d'un tags
int ajout_hierarchie(int i,char *tag) {
	json_object *jobj=read_json();
	json_object* j_tag;
	json_object *root=json_object_object_get(jobj,"tags");		
	if(json_object_object_get_ex(root,tag,&j_tag)==0) {
		printf("nooope\n");
		return FALSE;
	}
	json_object *obj_sub = json_object_object_get(j_tag,"subtags");
	json_object_array_add(obj_sub,json_object_new_int(i));
	int fd=open(CONF_PATH,O_WRONLY,0744);
	if(fd<0)
		perror("erreur open");
	struct stat sb;
	if(stat(CONF_PATH,&sb)<0)
		perror("erreur stat");
	write(fd,json_object_to_json_string(jobj),strlen(json_object_to_json_string(jobj)));
	close(fd);
	return TRUE;
}

//vérifie si un tag existe dans le json
int exist_tag_json(char *tag,int id) {
	json_object *jobj=read_json();
	json_object* j_tag;
	json_object *root=json_object_object_get(jobj,"tags");
	if(json_object_object_get_ex(root,tag,&j_tag)==0) {
		printf("pas là\n");
		write_json(tag,id,jobj);
		return id;
	}
	else{
		json_parse(j_tag);
		json_object *obj_Id = json_object_object_get(j_tag,"id");
		id=json_object_get_int(obj_Id);
		printf("id : %d \n",json_object_get_int(obj_Id));
		//t=check_hierarchie(tag);
		//printf("okay déjà là\n");
		return id;
	}
}

//renvoie le tag associé à l'id:
char * get_tag(int id){
	struct json_object_iterator it;
 	struct json_object_iterator itEnd;
	json_object *jobj=read_json();
	json_object* check_tag;
	json_object *root=json_object_object_get(jobj,"tags");
	const char *tag;
	it = json_object_iter_begin(root);
 	itEnd = json_object_iter_end(root);
	while (!json_object_iter_equal(&it, &itEnd)) { 	
		tag=json_object_iter_peek_name(&it);
		check_tag=json_object_object_get(root,tag);
		if(id==json_object_get_int(json_object_object_get(check_tag,"id")))
			return (char *)tag;
		
		json_object_iter_next(&it);
	}
	return NULL;
}


//supprime du tableau oui tous les subtags de not si il en a
int *is_a_sub(int *yes,int *len_y,int *not,int len_n){
	int i,j;
	json_object* j_tag;
	struct json_object* obj;
	json_object *sub;
	json_object *root;
	//int *ids;
	int len;
	malloc(sizeof(int));
	for(i=0; i<len_n; i++){
		for(j=0; j<*len_y;j++){
			obj =read_json();
			root=json_object_object_get(obj,"tags");
			j_tag=json_object_object_get(root,get_tag(not[i]));
			sub=json_object_object_get(j_tag,"subtags");
			len=json_object_array_length(sub);
			if(len>0){
				if(id_in(yes[j],sub)==TRUE){
					if(*len_y-1>0){
						memcpy(yes+j,yes+j+1,(*len_y-j)*sizeof(int));
						*len_y=*len_y-1;
					}
					else return NULL;
				}
			}
		}
	}
	return yes;
}
						

//appelé pour find_t				
int sub(int id1,int id2){
	json_object* j_tag;
	struct json_object* obj;
	json_object *sub;
	json_object *root;
	obj =read_json();
	root=json_object_object_get(obj,"tags");
	j_tag=json_object_object_get(root,get_tag(id1));
	sub=json_object_object_get(j_tag,"subtags");
	int len=json_object_array_length(sub);	
        if (len<=0)
		return FALSE;
	return id_in(id2,sub);
}
			
				
				
				
