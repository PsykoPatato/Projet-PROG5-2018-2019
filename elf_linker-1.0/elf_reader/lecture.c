#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include "header.h"
#include "section.h"
#include "contenuS.h"
#include "symbole.h"
#include "relocation.h"
#include "elf32_file.h"

Elf32_Sym *symb;
char *nomSym;


int main(int argc, char **argv){
  int option = 0;
  char* char_section = ' ';
  FILE *f;

  if(argc < 3){
     printf("Saisir option + nom de fichier:\
     \n -h: pour affichage du Header \n -S: pour affichage de la table des sections\
    \n -x: suivi de num/nom section pour afficher le contenu d'une section\n");
     exit(1);
  }

  char *char_option = argv[1];
  if(strcmp(char_option, "-h") == 0)
    option = 1;
  else if(strcmp(char_option, "-S") == 0)
    option = 2;
  else if(strcmp(char_option, "-x") == 0)
    option = 3;
  else if(strcmp(char_option, "-s") == 0)
    option = 4;
  else if(strcmp(char_option, "-r") == 0)
    option = 5;
  else
    printf("option invalide!\n");

  if(option == 3){
    char_section = argv[2];
    f = fopen(argv[3], "r");
  }
  else{
    f = fopen(argv[2], "r");
  }
  assert(f != NULL);

  Elf32_File *elf32_file = initElf32_File(f);

  switch(option){
    case 1:
      affichageHeader(elf32_file);
      break;
    case 2:
      afficheTableSection(f, elf32_file);
      break;
    case 3:
      affichageSection(f, elf32_file, char_section);
      break;
     case 4:
      afficheTableSymbole(f, elf32_file);
      break;
     case 5:
      afficheReadressage(f, elf32_file);
      break;
    default:
      break;
  }

  fclose(f);
  free(elf32_file->ehdr);
  free(elf32_file->shdr);
  free(elf32_file);
  return 0;
}
