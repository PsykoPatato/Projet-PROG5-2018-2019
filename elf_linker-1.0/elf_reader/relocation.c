#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include "section.h"
#include "relocation.h"


void reloc(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr * shdr, Elf32_Sym *symb, char *nomSec, char *nomSym){
  int entrees=0; int no = 0;//pr le cas ou pas de fichier de reloc
  Elf32_Rel *rel;
  int i = 0;
  printf("\n");
  while(i < ehdr->e_shnum ){
 //------pour chaque section Rel-----------
    if(shdr[i].sh_type == 9){  //SHT_REL
      no++;
      entrees = shdr[i].sh_size / sizeof(Elf32_Rel);
      rel = malloc(sizeof(Elf32_Rel) * entrees);
      assert(rel != NULL);

      //-----lecture de tous les champs------------------
      fseek(f, shdr[i].sh_offset, SEEK_SET);
      for(int j=0; j<entrees; j++){
        inverse(f, &rel[j].r_offset, sizeof(rel[j].r_offset));
        inverse(f, &rel[j].r_info, sizeof(rel[j].r_info));
      }

      //-------Ecriture-----------------------------
      printf("Section de réadressage '%s' à l'adresse de décalage 0x%x contient %d entrées:\n", nomSec+shdr[i].sh_name, shdr[i].sh_offset, entrees);

      printf("%8.8s \t", "Decalage");
      printf("%8.8s \t","Info");
      printf("%15.15s \t","Type");
      printf("%8.8s \t","Val.-sym");
      printf("%20.20s \n","Noms.-symboles");


      for(int j=0; j<entrees; j++){
        printf("%8.8x \t", rel[j].r_offset);
        printf("%8.8x \t", rel[j].r_info);
        switch ((unsigned char)rel[j].r_info){
          case R_ARM_NONE: printf("%15.15s \t","R_ARM_NONE"); break;
          case R_ARM_PC24: printf("%15.15s \t","R_ARM_PC24"); break;
          case R_ARM_ABS32: printf("%15.15s \t","R_ARM_ABS32"); break;
          case R_ARM_REL32: printf("%15.15s \t","R_ARM_REL32"); break;
          case R_ARM_PLT32: printf("%15.15s \t","R_ARM_PLT32");break;
          case R_ARM_CALL: printf("%15.15s \t","R_ARM_CALL");break;
          case R_ARM_JUMP24: printf("%-15s \t","R_ARM_JUMP24");break;
          case R_ARM_THM_JUMP24: printf("%15.15s \t","R_ARM_THM_JUMP24");break;
          case R_ARM_BASE_ABS: printf("%15.15s \t","R_ARM_BASE_ABS");break;
          default: printf("%15.15s \t", "type non connu!"); break;
        }
        printf("%8.8x \t", (ELF32_R_SYM(rel[j].r_info) >> 8) ); //Val.-symb
        //----------NOM----------------------------------------
        int ind = (int)(rel[j].r_info >> 8);
        if(symb[ind].st_info  == STT_SECTION )
           printf("%20.20s \n", nomSec + shdr[ symb[ind].st_shndx ].sh_name );
        else
           printf("%20.20s \n", nomSym + symb[ind].st_name);

      }
      printf("\n");
      free(rel);
    }

    i++;
  }
  if(no == 0){
    printf("Il n'y a pas de réadressage dans ce fichier.\n");
  }


}
