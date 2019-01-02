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

Elf32_Sym *symb;
/*void test(int taille){
  symb = malloc(sizeof(Elf32_Sym) * taille);
  if(symb == NULL)
    printf("erroorrr!\n");
}*/

int main(int argc, char **argv){
   int c;

   char *section;
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
   else if(strcmp(op, "-x") == 0)
      c = 3;
  else if(strcmp(op, "-s") == 0)
      c = 4;
  else if(strcmp(op, "-r") == 0)
          c = 5;
   else
     printf("option invalide!\n");
     //exit(2);
//---------------------------------------------------------
   if(c==3){
     section = argv[2];
     f = fopen(argv[3], "r");
   }
   else{
     f = fopen(argv[2], "r");
   }
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
  char *nom = malloc( shdr[ehdr->e_shstrndx].sh_size);
  assert(nom != NULL);
  fseek(f, shdr[ehdr->e_shstrndx].sh_offset, SEEK_SET);
  fread(nom, t, 1, f);

//----------------+Etape 3+-------//recup du nom de section et de son indice------------------------
  int i, v = atoi(section);
  int ind;
  char *nomS; //nom de la section à afficher
  if(v > 0 && v < ehdr->e_shnum){ //alors s est le num de la section
   ind = v;
   nomS = nom+shdr[v].sh_name;
  }
  else{   //s est le nom de la section
   i=0;
   while (i < ehdr->e_shnum && strcmp(section, nom+shdr[i].sh_name)) {
     i++;
   }
   if(i == ehdr->e_shnum && c==3){
     printf("nom de section invalide!\n");
     ind = 0;
     nomS = "";
     //exit(3);
   }
   else{
     nomS = section;
     ind = i;
   }
 }

 //-------Etape 4:--creation table de symboles--------------------------------------------------------------
   //recherche de l'indice de ".symtab" dans la table des sections
   int iSym; //indice de .SYMTAB
   int taille; //taille de la table de symboles
   i = 0;
   while(i < ehdr->e_shnum  &&  shdr[i].sh_type != 2){
     i++;
   }
   if(i == ehdr->e_shnum){
     printf("indice de « .symtab » non trouvé!\n");
     iSym = 0;
     //exit(4);
   }
   else{
     iSym = i;
   }

   taille = shdr[iSym].sh_size / shdr[iSym].sh_entsize;
   symb = malloc(sizeof(Elf32_Sym) * taille);
   assert(symb != NULL);
   //test(taille);
   readSymbole(f, ehdr, shdr, symb, iSym);

//-----------------------Etape 5---------------------------------------------
   /*int count = 0;
   i = 0;
   while(i < ehdr->e_shnum ){
     if(shdr[i].sh_type == 9){ //SHT_REL
       count++;
     }
     i++;
   }
  rel = malloc(sizeof(Elf32_Rel) * count);
  assert(rel != NULL);*/

  //printf("%d ", count);





   switch(c){
     case 1:
      printHeader(ehdr);
      break;
     case 2:
      printf("affichage de la table de section\n");
      printSection(f, ehdr, shdr, nom);
      break;
     case 3:
      //printf("affichage du contenu de la section %s:\n", argv[2]);
      contenuS(f, ehdr, shdr, ind, nomS);
      printf("\n");
      break;
     case 4:
      printf("Table de symboles « .symtab » contient %d entrées:\n", taille);
      printSymbole(f, ehdr, shdr, symb, iSym, nom);
      break;
     case 5:
      printf("Tables de réimplantation:\n");
      reloc(f, ehdr, shdr, symb, nom);

     default:
      break;
  }



  fclose(f);
  free(nom);
  free(ehdr);
  free(shdr);
  free(symb);

  return 0;
}
