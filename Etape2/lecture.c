#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include "header.h"
#include "section.h"


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
   else if(strcmp(op, "-S") == 0)
    c = 2;
   else
     printf("option invalide!\n");
     //exit(2);
//---------------------------------------------------------
 
   f = fopen(argv[2], "r");
   assert(f != NULL);
//------------------------------------------
   Elf32_Ehdr *ehdr = malloc(sizeof(Elf32_Ehdr));
   assert(ehdr != NULL);


   readHeader(f, ehdr);

   Elf32_Shdr *shdr = malloc(sizeof(Elf32_Shdr) *ehdr->e_shnum);
   assert(shdr != NULL);

   readSection(f, ehdr, shdr);


//-------------------------Etape 2---------------------------------
  //recup du tableau des noms de sections
  int t = shdr[ehdr->e_shstrndx].sh_size;
  char *nom = malloc(t);
  assert(nom != NULL);
  fseek(f, shdr[ehdr->e_shstrndx].sh_offset, SEEK_SET);
  fread(nom, t, 1, f);


//-------------------------------------------------------------------------------

   switch(c){
     case 1:
      printHeader(ehdr);
      break;
     case 2:
      printf("affichage de la table de section\n");
      printSection(f, ehdr, shdr, nom);
      break;

     default:
      break;
  }



  fclose(f);
  free(nom);
  free(ehdr);
  free(shdr);
  
  return 0;
}
