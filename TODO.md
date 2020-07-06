# Tâches à effectuer

## 1. Définir l'architecture:


proposition Eléonore:
* les tags seront organisés de façon à être globaux (avec structure inode)
* stockés dans des i-noeuds (on a plus de pistes de ce côté là dans le cours et me semble plus intuitif)
* pour le dernier point, je dirais une réaction direct à une modification du SGF? (mais je ne suis pas sûre sur ce point)

## 2. Se concentrer sur les différentes étapes:


* marquer un fichier avec un tag
* effacer un tag d'un fichier 
* lister les tags d'un fichier
* lister les fichiers possédant une combinaison de tags
* spécifier un lien hiérarchique entre deux tags

### a) Verifier la compatibilité avec notre SGF


* mv: si on modifie le nom ou si on le change d'endroit, les tags SUIVENT
* cp:la copie possède les mêmes tags que l'originel

#### les liens:

* lien physique:

ln → les tags ne concernent que le fichier de manières indépendante du lien physique

rm → supprime un lien physique, s'il en existe d'autres, les tags mrquent encore ces liens
sinon le fichier n'apparaît plus quand on appelle ses tags

* lien symbolique:

ln → les tags du lien symbolique seront les mêmes que ceux marquant le fichier référencé

## 3.Hierarchie

`xattr` 

utiliser Yaml

-----------------------------------------------------------------------------------------------------------
# TO DO

getxattr-> permet (en autres) de récupérer la taille de l'attribut
setxattr, lsetxattr, fsetxattr - set an extended attribute value -> créer un attribut tags? (stocké dans inode?)
- [ ] création des tags
- [ ] organiser la structure
- [ ] diviser les taĉhes (appel)
- [ ] organiser les fonction en commande (exemple tag add, tad remove ,tag searcg ,etc...) 
- [ ] hierarchie des tags-> sotcker un id crrespondant aux tags dans l'inode ,faisant références à un tag dans un stockage de la hierarchie des tags (comme ça si rouge est créé avant couleur, les fichiers déjà tagés rouge seront trouvé lorsqu'on cherche couleur)
------------------------------------------------------------------------------------
## 3.Utilité d'un inode?

### a) C'est quoi? (ref. wiki)

Créés lors de la création d'un fichier, ils contiennent de nombreuses informations sur les fichiers. La quantité d'inodes indique le nombre maximum de fichiers que le systeme de fichier peut contenir

* Le numéro d'inode est un entier unique pour le système de fichier dans lequel il est stocké. Le numéro d'inode d'un fichier `truc` peut être affiché avec la commande:

```ls -i truc ```

### b) Ca contient quoi?  (ref.Design of the Unix Operating System by Maurice Bach)

* Les identifiant propriétaires du fichier: divisés en propriétaire individuel et group. On y définit l'ensemble des utilisateurs ayant accès au fichier. Le superutilisateur a accès à tous les fichiers du systeme.
* Le type du fichier. Files may be of type regular, directory, character or block special, or FIFO (pipes)
* Les permissions d'accès au fichier. Le syteme protège les fichiers en faisant 3 classes:
    * le propriétaire
    * le groupe de propriétaires
    * et les autres utilisateurs
    
Chaque classe a accès aux droits read,write,ex du fichier qui peuvent être réglé de manière individuelle (ref. `chmod`)
Comme les dossiers ne peuvent pas être exécutés, la permission d'execution pour un dossier donne le droite de rechercher un nom de fichier dans le dossier.

* File access times: donne time du fichier de la dernière fois où il a été modifié,le dernier accès et quand le inode a été modifé en dernier
* Le nombre de liens du fichier, représenté par le nombre de noms que le fichier a dans hierarchie des dossiers  (cf. Point expliquant les liens en détail)
* "Table des matières" des adresses disque des données dans un fichier. Bien qu'un utilisateur traite les données d'un fichier comme un flux logique d'octets, le kernel enregistre les données sur les blocs de disque non contigus. Le inode identifie les blocs de disque qui contiennent les données du fichier.

 `contigu : Qui touche à quelque chose, à un lieu ; qui en est voisin `

`allocation non-ontigüe: pagination `

* FILE SIZE. Les données dans un fichier adressable (indexable?) par un nombre de bytes à partir du début du fichier, commençant par le décallage du byte 0, et la taille du fichier est 1 supérieur au décalage d'octets le plus élevé des données du fichier. 

Par exemple, si un utilisateur crée un fchier et écrit seulement 1 octet de données à 1 000 octet de décalage dans le fichier, la taille du fichier est de 1001 octets.

Exemple de Inode :

``` terminal
owner mjb
group os
type regular file
perms rwxt-xr-x
accessed Oct 23 1984 1:45 P.M
modified Oct 22 1984 10:30 A.M
inode Oct 23 1984 1:30 P.M
size 6030 bytes
disk addresses
```
Ici le inode est un regular file, avec "mbf" comme propriétaire qui contient 6030 bytes. Le system permet "mjb" de read,write ex, les membres du group "os" et les autre peuvent uniquement read ou ex, pas write.
Pour les dates c'est obvious . Le kernel encode toutes ces informations sur le inode. Notez la distinction entre l'écriture du contenu d'un inode sur le disque et l'écriture du contenu d'un fichier sur le disque. Le contenu d'un fichier ne change que lors de son écriture. Le contenu d'un inode change lors de la modification du contenu d'un fichier ou lors de la modification de son propriétaire,de sa permission ou des paramètres de lien.
Changer le contenu d'un fichier implique automatiquement un changement dans l'inode, mais les changements dans l'inode n'impliquent pas le changement de contenu du fichier.

ICI `in-core` signifie que les inodes traités par le kernel sont copié sur la RAM de celui-ci à partir du disque physique.

[voir aussi][http://140.120.7.21/LinuxKernel/LinuxKernel/node42.html]

The in-core copy of the inoode contains the following fields in addition to thefields of the disk inode:

* Le status du in-core inode indique **SI** :
    * le inode est locked
    * le processus est en train d'attendre que le inode soit unlocked
    * la représentation in-core des inode diffère de la copie du disque suite à la modification des données dans le inode
    * la représentation in-core du fichier diffère de la copie du disque lors de la modification des données du fichier
    * le fichier est un mount point 
    
`point de montage (mount point): En informatique, un point de montage est un répertoire à partir duquel sont accessibles les données se trouvant sous forme d'un système de fichiers sur une partition de disque dur ou un périphérique. Plus simplement, c'est le dossier qui permet d'accéder au contenu d'un disque dur, clé USB, lecteur DVD, ou autre périphérique de stockage. `

* Le numéro logique (numéro assigné par le kernel) qui contient le fichier
* Le numéro inode. Le kernel identifie le nombre d'un inode de disque par la position dans le tableau linéaire du disque.Le disque inode n'a pas besoin de ce paramètre.
* Des pointeurs vers d'autres inodes.
* Un nombre de référence indiquant le nombre d'instances du fichier qui sont actives (such as when opened).

De nombreux paramètres de in-core inode sont semblables aux paramètres de l'en-tête tampon et la gestiondes inodes est similaire à la gestion des tampons.

* Le verouillage du inode,une fois défini,empêche les autres processus d'accéder au inode; d'autres processus définissent un "drapeau" dans le inode lorsqu'ils tentent d'y accéder pour indiquer qu'ils doivent se réveiller lorsque le errou est libéré.

Le kernel définit d'autres indicateurs pour indiquer les écarts entre les inodes disque et les in-core copy(expliqué plus haut lors de la définition d'in-core).

Lorsque le noyau doit enregistrer les modifications apportées au ficher ou au inode, il écrit la in-core copy après avoir examiné ces indicateurs.

La différence la plus frappante entre un in-code inode et un en-tête de tampon est le nombre de in-core références ,qui compte le nombre d'instances actives du fichier.

Un inode est actif losqu'un processus l'alloue, comme lors de l'ouverture d'un fichier.

Un inode n'est sur la free-list que si son nombre de références est 0, ce qui signifie que le kernel peut réallouer le inode in-core vers un autre inode disque. 

* La free-list sert de cache d'inodes inactifs: si un processus tente d'accéder à un fichier dont le inode n'est pas actuellement dans le pool de inode in-core, le kernel réalloue une inode in-core de la free-list.

D'un autre côté, un tampon ne fait pas de référence, il est sur la free-list seuliement s'il est unlocked.

### c) Les algorithmes vus en cours

les algos se trouvent dans le cours, ici ils sont simplement expliqués

##### 1) iget 

Iget permet de charger en mémoire vive l'inode spécifié par le numéro de périphérique et le numéro d'inode sur celui-ci. Il renvoie l'adresse du descripteur de l'inode correspondant.

- Entrée: numéro d'inode
- Sortie inode en mémoire locked

Est utilisé vers le début des appels system losqu'un processus accède pour la première fois à un fichier. Le inode in-core contient des informations à jour de l'état du fichier. Le kernel dévérouille le inode avant de revenir de l'appel system afin que les autres appels system puissent accéder au inode s'ils le souhaitent.

##### 2) iput

Puts an inode, dropping its usage count. If the inode use count hits zero, the inode is then freed and may also be destroyed. 

- Entrée: référence vers un inode en mémoire
- Sortie: rien ----> Libération de l'inode en mémoire 

##### 3) namei

conversion of a path name to an inode

- Entrée: nom de fichier (chemin absolu ou relatif)
- Sortie: inode en mémoire vérouillé correspondant

##### 4) ialloc

- Entrée: systeme de fichiers
- Sortie: inode en mémoire vérouillé(avec création d'un inode sur disque)

##### 5) ifree

- Entrée: numéro d'inode
- Sortie: rien(avec libération de l'inode sur disque correspondant)

### d) Appels système:

pour plus de détails faire man

*  Modifier les autorisations d'accès à un fichier.

    `chmod` : modifie  les  permissions du fichier indiqué dont le nom est
         fourni  dans  path,  qui  est  déréférencé  s’il  s’agit  d’un   lien
         symbolique.

    `fchmod` : modifie  les  permissions  du  fichier  référencé  par  le
         descripteur de fichier ouvert fd.
    
    ``` c
        #include <sys/stat.h>

       int chmod(const char *pathname, mode_t mode);
       int fchmod(int fd, mode_t mode);
    ``` 

`utime`: Modifier la date d'accès ou de modification d'un inœud  

[lien man utime ][http://manpagesfr.free.fr/man/man2/utime.2.html]


``` c
#include <sys/types.h>


#include <utime.h>

int utime(const char *filename, const struct utimbuf *times);

#include <sys/time.h>

int utimes(const char *filename, const struct timeval times[2]);
```

* Tronquer un fichier à une longueur donnée

    `troncate` : tronquent le fichier référencé par path à une longueur d'exactement length octets. 
    
    `ftroncate`: tronquent le fichier référencé par par le descripteur fd à une longueur d'exactement length octets.
    
    ``` c
    #include <unistd.h>
    #include <sys/types.h>

    int truncate(const char *path, off_t length);   
    int ftruncate(int fd, off_t length); 
    ```
    
` mkdir`: mkdir - Créer un répertoire  
    
``` c
#include <sys/stat.h>
#include <sys/types.h>

int mkdir(const char *pathname, mode_t mode);
```
`rmdir` : Supprimer des répertoires vides

``` c
 #include <unistd.h>

int rmdir(const char *pathname);   
```
`link` , `unlink` : voir 4.Les  liens

`symlink` : Créer un nouveau nom pour un fichier

``` c
#include <unistd.h>

int symlink(const char *cible, const char *nom); 
``` 
`rename` : changer le nom ou l'emplacement d'un fichier

*  renomme un fichier, en le déplaçant dans un autre répertoire si nécessaire. Tous les autres liens matériels vers le fichier (créés avec link(2)) restent inchangés. Les descripteurs de fichier ouverts sur oldpath restent également inchangés. 
* Si oldpath et newpath sont des liens matériels existants faisant référence au même fichier, rename() ne fait rien et renvoie un code de réussite.
* Si newpath existe mais que l'opération échoue pour une raison quelconque, rename() garantit la présence d'une instance de newpath en place. 
* oldpath peut indiquer un répertoire. Dans ce cas, newpath doit soit ne pas exister, soit être un répertoire vide. 
* Si oldpath correspond à un lien symbolique, le lien est renommé ; si newpath correspond à un lien symbolique, le lien est écrasé.  

``` c
#include <stdio.h>

int rename(const char *oldpath, const char *newpath);   
```  
* Dupliquer un descripteur de fichier  :
 
    `dup ` :  utilise le plus petit numéro inutilisé pour le nouveau descripteur.

   `dup2 ` : transforme newfd en une copie de oldfd, fermant auparavant newfd si besoin est, mais veuillez noter ceci :

*   
    * Si oldfd n'est pas un descripteur de fichier valide, l'appel échoue et newfd n'est pas fermé. 
    * Si oldfd est un descripteur de fichier valide et si newfd a la même valeur que oldfd, dup2() ne fait rien et renvoie newfd. 

```c
#include <unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);
``` 

`exec` [][http://manpagesfr.free.fr/man/man3/exec.3.html]

* Obtenir l'état d'un fichier (file status) : (cf. TP7)

    `stat` : récupère l'état du fichier pointé par path et remplit le tampon buf.
    
    `lstat`: est identique à `stat()`, sauf que si path est un lien symbolique, il donne l'état du lien lui-même plutôt que celui du fichier visé. 

    ``` c
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    
    int stat(const char *path, struct stat *buf);
    int fstat(int fd, struct stat *buf);
    `
    
* Ouvrir et fermer un repertoire: 

    `opendir`: Ouvrir un répertoire
    `closedir `: Ferme répertoire
    
    ``` c
    #include <sys/types.h>
    #include <dirent.h>

    DIR *opendir(const char *name);
    DIR *closedir(const char* name);
    ```
`readdir` : Lire une entrée de répertoire  

``` c
#include <linux/types.h>
#include <linux/dirent.h>

int readdir(unsigned int fd, struct dirent *dirp,
            unsigned int count);

//exemple lister directory
while((fd= readdir(rep)) !=NULL){
		printf("%s  ",fd->d_name);
	}
``` 


    
## 4. Les liens

##### 1) Link

The syntax for the link system call is:

``` c
link(source file name, target file name);
```
`source file name` : nom d'un fichier existant

`target file name` : le nouveu(addionel) nom du fichier après l'appel de la fonction

Le fichier system contient le path name pour chaque lien que le fichier a et le processus peut accéder au fichier via chacun des path names.
Le kernel ne sait pas lequel était le nom originel, donc aucun nom a un traitement plus spécial que les autres.

* Exemple:
    ```c 
    link ("/usr/src/uts/sys", "/usr/include/sys");
    link ("/usr/include/realfile.h", "/usr/src/uts/sys/testfile.h");
    ``` 
    Les trois parth name se réfèrent au même fichier.

Le kernel permet uniquement à un superutilisateur de lier des répertoires, simplifiant ainsi le coding des programmes qui traversent l'arborescence du sytème de fichiers. Si des utilisateurs arbitraires pouvaient lier des répertoires, les programmes conçus pour traverser la hiérarchie des fichiers devraient se soucier de se retrouver dans une boucle infinie si un utilisateur devait lier un répertoire à un node name en dessous dans la hiérarchie.
Les superutilisateurs sont ,normalement, plus prudants à propos de tels liens. 
`mkdir` a permis d'éliminer le besoin de faire des liens entre répertoires

##### 2) Unlink

The unlink system call removes a directory entry for a file. The syntax for theunlink call is:

``` c
unlink(pathname);
``` 

`pathname` : nom du fichier qui doit être unlinked de la directory hierarchy

`à finir`

## 5. Super block

Un super block consiste en ces champs :

* la taille du systeme de fichiers
* le nombre de bloque libre dans le système de fichier
* une liste de free blocks disponible sur le système de fichier
* l'index du prochain free block dans la liste free block
* la taille de la liste d'inode
* le nombre d'inode liste dans le système de fichier
* une liste d'inodes libres dans le système de fichier
* l'indec du prochain inode libre dans la listre des inodes libres
* le champ véruillé du free block et de la liste des inodes disponibles
* un drapeau indiquant que le super block a été modifié

----> voir ialloc,ifree