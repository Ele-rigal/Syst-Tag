Nous avons décidé de structurer notre programme de la façon suivante: 

* toutes les fonctions relatives à la gestion des tags se trouvent dans le fichier 'tag.c'. Il s'agit des méthodes permettant d'ajouter ou supprimer un tag sur un fichier.
On trouve également dans ce fichier, le code permettant de lire ou récupérer les attributs (si c'est un répertoire, un lien ou un fichier ordinaire) ou les tags d'un fichier.

* comme nous stockons les tag dans un fichier json, nous avons créé le fichier 'interpreter.h' où se trouvent toutes les fonctions permettant de créer, lire, éditer le fichier json comprenant les tags.

* pour finir, le fichier main contient la fonction qui va lancer le programme, ainsi que la fonction qui gère l'appel à 'help'

nous aurions egalement aime implementer une recherche globale (recursive) mais le temps nous a manque