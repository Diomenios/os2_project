#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

/** Permet de convertir un temps donne en seconde, en un temps de type :
*   XX hour, XX minutes and XX seconds
*
* @param int temps     le temps en secondes qui va etre convertis
*
* @return char* temps  la conversion, etant sauvegardee en memoire.  Necessitera un
*                      free() par la suite
*
*/
char* convertion(int temps);
#endif // HELPER_H_INCLUDED
