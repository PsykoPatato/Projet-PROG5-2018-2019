#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include "section.h"
#include "symbole.h"

//Elf32_Sym *symb;

void readSymbole(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr * shdr,  Elf32_Sym *symb, int ind){
  int i;
  int offset =  shdr[ind].sh_offset;
  int taille = shdr[ind].sh_size / shdr[ind].sh_entsize;

  //symb = malloc(sizeof(Elf32_Sym) * taille);
  //assert(symb != NULL);

  fseek(f, offset, SEEK_SET); //On se place au début de la table des symboles

  for(i=0; i<taille; i++){
    inverse(f, &symb[i].st_name, sizeof(symb[i].st_name));
    inverse(f, &symb[i].st_value, sizeof(symb[i].st_value));
    inverse(f, &symb[i].st_size, sizeof(symb[i].st_size));
    inverse(f, &symb[i].st_info, sizeof(symb[i].st_info)); //fournit le type et attributs de liaison(Lien)
    inverse(f, &symb[i].st_other, sizeof(symb[i].st_other)); //fournit la visibilité du symbole(Vis)
    inverse(f, &symb[i].st_shndx, sizeof(symb[i].st_shndx)); //Indice de la section contenant le symbole
  }
}


void printSymbole(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr *shdr,  Elf32_Sym *symb, int ind, char *nomSym){   //nom est le nom de sections


//------------------Affichage -----------------------------------------------------------

   int taille = shdr[ind].sh_size / shdr[ind].sh_entsize;

   printf("%4.4s \t", "Num: ");
   printf("%8.8s \t","Valeur");
   printf("%4.4s \t","Tail");
   printf("%8.8s \t","Type");
   printf("%6.6s \t","Lien");
   printf("%8.8s \t","Vis");
   printf("%3.3s \t","Ndx");
   printf("%5.5s \n","Nom");

   for( int i=0; i<taille; i++){
     printf("%d:\t", i); //Num
     printf("%8.8x \t", symb[i].st_value); //Valeur
     printf("%d \t", symb[i].st_size); //Tail

     //--------TYPE------------------------
     unsigned type = symb[i].st_info & 0xf;

       switch (type){
         case STT_NOTYPE:
             printf("%8.8s \t", "NOTYPE");
             break;
         case STT_OBJECT:
             printf("%8.8s \t", "OBJECT");
             break;
         case STT_FUNC:
             printf("%8.8s \t","FUNC");
             break;
         case STT_SECTION:
             printf("%8.8s \t", "SECTION");
             break;
         case STT_FILE:
             printf("%8.8s \t", "FILE");
             break;
         case STT_LOPROC:
             printf("%8.8s \t", "LOPROC");
             break;
         case STT_HIPROC:
             printf("%8.8s \t", "HIPROC");
             break;
       }

      //--------LIEN--------------------------------------------
      unsigned  bind = symb[i].st_info >> 4;
       switch (bind){
         case STB_LOCAL:
             printf("%6.6s \t","LOCAL");
             break;
         case STB_GLOBAL:
             printf("%6.6s \t","GLOBAL");
             break;
         case STB_WEAK:
             printf("%6.6s \t","WEAK");
             break;
         case STB_LOPROC:
             printf("%6.6s \t","LOPROC");
             break;
         case STB_HIPROC:
             printf("%6.6s \t","HIPROC");
             break;
       }

 //------------------Vis-----------------------------
     switch (symb[i].st_other){
       case STV_DEFAULT :
         printf("%8.8s \t","DEFAULT");
         break;
       case STV_INTERNAL :
         printf("%8.8s \t","INTERNAL");
         break;
       case STV_HIDDEN :
         printf("%8.8s \t","HIDDEN");
         break;
       case STV_PROTECTED :
         printf("%8.8s \t","PROTECTED");
         break;
       default :
         printf("%8.8s \t","ERREUR");
         break;
     }

      //---------------------Ndx--------------
      switch (symb[i].st_shndx){
         case SHN_UNDEF:
             printf("%3.3s \t","UND");
             break;
         case SHN_ABS:
             printf("%3.3s \t","ABS");
             break;
         default:
             printf("%d \t", symb[i].st_shndx);
             break;
      }
    //----------------NOM--------------------------
      printf("%5.5s\n", nomSym + symb[i].st_name);
   }





}
