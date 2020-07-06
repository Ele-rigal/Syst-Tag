#include "parser.h"
#include <json-c/json.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#ifndef CONF_PATH
# define CONF_PATH "tags.json"
#endif

void create_json();

char *getfile(char *name);

json_object *read_json();

void write_json(const char *tag,int i,json_object *jobj);

int id_in(int id,struct json_object *t);

int *check_hierarchie(char *tag);

int ajout_hierarchie(int i,char *tag);

int exist_tag_json(char *tag,int id);

int new_id();

char *get_tag(int id);

int *is_a_sub(int *yes,int *len_y,int *not,int len_n);

int sub(int id1,int id2);
