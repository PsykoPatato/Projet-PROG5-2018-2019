#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include "header.h"



int main(int argc, char **argv){
   int c;
   FILE *f;
   if(argc < 3){
     printf("Saisir option + nom de fichier:\
     \n -h: pour affichage du Header \n -S: pour affichage de la table des sections\
    \n -x: suivi de num/nom section pour afficher le contenu d'une section\n");
     exit(1);
   }
//--------------//recup de l'option i pour etape i---------------------------------------------------

   char *op = argv[1];
   if(strcmp(op, "-h") == 0)
    c = 1;
   else
     printf("option invalide!\n");
     //exit(2);



   f = fopen(argv[2], "r");
   assert(f != NULL);
//------------------------------------------
   Elf32_Ehdr *ehdr = malloc(sizeof(Elf32_Ehdr));
   assert(ehdr != NULL);


   readHeader(f, ehdr);

   
//-------------------------------------------------------------------------------

   switch(c){
     case 1:
      printHeader(ehdr);
      break;
    
     default:
      break;
  }



  fclose(f);
  free(ehdr);
 
  return 0;
}
