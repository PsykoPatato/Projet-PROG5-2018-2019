#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include "section.h"

char *type(int val){

  switch(val){
      case 1:
          return "PROGBITS";
      case 2:
          return "SYMTAB ";
      case 3:
          return "STRTAB ";
      case 4:
          return "RELA  ";
      case 5:
          return "HASH  ";
      case 6:
          return "DYNAMIC";
      case 7:
          return "NOTE  ";
      case 8:
          return "NOBITS";
      case 9:
          return "REL  ";
      case 10:
          return "SHLIB";
      case 11:
          return "DYNSYM";
      case 0x70000000:
          return "LOPROC";
      case 0x7fffffff:
          return "HIPROC";
      case 0x80000000:
          return "LOUSER";
      case 0xffffffff:
          return "HIUSER";
      case 0x70000003:
          return "ARM_ATTRIBUTES";
      default:
          return "NULL  ";
   }
}

void inverse(FILE *f, void *t, int k){
  int  i, c;
  char s[k];

    for(i=0; i<k; i++){
      c = fgetc(f);
      s[i] = c;
    }
    for(i=k-1; i>=0; i--){
      *((char *)t++) = s[i];
    }
}


void readTableSection(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr *shdr){
  int i;
  fseek(f, ehdr->e_shoff, SEEK_SET); //On se place au début de section header table
  for(i = 0; i < ehdr->e_shnum; i++){
    inverse(f, &shdr[i].sh_name, sizeof(shdr[i].sh_name));
    inverse(f, &shdr[i].sh_type, sizeof(shdr[i].sh_type));
    inverse(f, &shdr[i].sh_flags, sizeof(shdr[i].sh_flags));
    inverse(f, &shdr[i].sh_addr, sizeof(shdr[i].sh_addr));
    inverse(f, &shdr[i].sh_offset, sizeof(shdr[i].sh_offset));
    inverse(f, &shdr[i].sh_size, sizeof(shdr[i].sh_size));
    inverse(f, &shdr[i].sh_link, sizeof(shdr[i].sh_link));
    inverse(f, &shdr[i].sh_info, sizeof(shdr[i].sh_info));
    inverse(f, &shdr[i].sh_addralign, sizeof(shdr[i].sh_addralign));
    inverse(f, &shdr[i].sh_entsize, sizeof(shdr[i].sh_entsize));
  }
}



void printTableSection(Elf32_Ehdr *ehdr, Elf32_Shdr *shdr, char *nom){
  printf("Il y'a %u en-tetes de section debutant a l'adresse de decalage 0x%x:\n", (unsigned)ehdr->e_shnum, ehdr->e_shoff);
  printf("en-tetes de section:\n");
  printf(" %4.4s " ,"[Nr]");
  printf("%20.20s \t ","Nom             ");
  printf("%15.15s \t ","  Type");
  printf("%8.8s \t", "Adr    ");
  printf("%6.6s \t","Decala.");
  printf("%6.6s \t","Taille");
  printf("%2.2s \t","ES");
  printf("%3.3s \t","Fan");
  printf("%2.2s \t","LN");
  printf("%3.3s \t","Inf");
  printf("%2.2s \n","Al");

  for(int i = 0 ; i < ehdr->e_shnum; i++){
    char flags[4] = "";
    strcpy(flags, "");
    if(shdr[i].sh_flags & SHF_WRITE)
      strcat(flags, "W");
    if(shdr[i].sh_flags & SHF_ALLOC)
      strcat(flags, "A");
    if(shdr[i].sh_flags & SHF_EXECINSTR)
      strcat(flags, "X");
    if(shdr[i].sh_flags &  SHF_MERGE)
      strcat(flags, "M");
    if(shdr[i].sh_flags &  SHF_STRINGS)
      strcat(flags, "S");
    if(shdr[i].sh_flags & SHF_INFO_LINK)
      strcat(flags, "I");
    if(shdr[i].sh_flags & SHF_LINK_ORDER)
      strcat(flags, "L");
    if(shdr[i].sh_flags & SHF_OS_NONCONFORMING)
      strcat(flags, "O");
    if(shdr[i].sh_flags & SHF_GROUP)
      strcat(flags, "G");
    if(shdr[i].sh_flags & SHF_TLS)
      strcat(flags, "T");
    if(shdr[i].sh_flags & SHF_MASKOS)
      strcat(flags, "o");
    if(shdr[i].sh_flags & SHF_ORDERED)
      strcat(flags, "x");
    if(shdr[i].sh_flags & SHF_EXCLUDE)
      strcat(flags, "E");
    if(shdr[i].sh_flags & SHF_MASKPROC)
      strcat(flags, "p");

    printf(" [%2.2d] ", i);
    printf("%20.20s\t ",(char *)nom+shdr[i].sh_name );
    printf("%15.15s \t", type((int)shdr[i].sh_type));
    printf("%8.8x \t", shdr[i].sh_addr);
    printf("%6.6x \t", shdr[i].sh_offset);
    printf("%6.6x \t", shdr[i].sh_size);
    printf("%2.2x \t", shdr[i].sh_entsize);
    printf("%2.2s \t ", flags);
    printf("%2.2d \t ", shdr[i].sh_link);
    printf("%2.2d \t", shdr[i].sh_info);
    printf("%2.2d \n", shdr[i].sh_addralign);
  }
  printf("Clé des fanions :\n W (écriture), A (allocation), X (exécution), M (fusion), S (chaînes)\n I (info), L (ordre des liens), G (groupe), T (TLS), E (exclu), x (inconnu)\n O (traitement additionnel requis pour l'OS) o (spécifique à l'OS), p (spécifique au processeur)");
}

void printSection(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr *shdr, char* nom, char* char_section){
  int num_section = atoi(char_section);
  char *nom_section; //nom de la section à afficher
  if(num_section > 0 && num_section < ehdr->e_shnum){ //alors s est le num de la section
   nom_section = nom+shdr[num_section].sh_name;
  }else{   //s est le nom de la section
   int i=0;
   while (i < ehdr->e_shnum && strcmp(char_section, nom+shdr[i].sh_name)) {
     i++;
   }
   if(i == ehdr->e_shnum){
     printf("nom de section invalide!\n");
   }else{
     nom_section = char_section;
     num_section = i;
   }
  }

  fseek(f, shdr[num_section].sh_offset, SEEK_SET); //on se place au debut de la section à lire

  int taille_section = shdr[num_section].sh_size;
  if(taille_section > 0) {
    int tab[taille_section]; //tab contiendra les size octets de la section à lire
    //initialisation de tab à 0
    int i=0;
    while(i < taille_section) {
      tab[i] = 0;
      i++;
    }
    //copie de chaque octet de la section dans tab
    i = 0;
    while(i < taille_section){
      fread(&tab[i], 1, 1, f);
      i++;
    }

    printf("Vidange hexadécimale de la section « %s » :", nom_section);

    i = 0;
    while(i < taille_section){
      if(i % 16 == 0){             //1e colonne
        printf("\n 0x%.8x ", i);
      }
      if(i % 4 == 0){     //fin colonne de 4 octets
        printf(" ");
      }
      printf("%.2x", tab[i]);  //affichage de l'octet i
      if(i % 16 == 15){  //affichage derniere colonne
        int j = i - 15;
        printf(" ");
        while (j < i + 1) {
          if (tab[j] >= 32 && tab[j] <= 126) {
            printf("%c", tab[j]);
          }else{
            printf(".");
          }
          j++;
        }
      }
      i++;
    }

    if(i % 16 != 15){  //affichage de la derniere colonne si taille non modulo 16
      int j = i - (i % 16);
      int k = i;
      while(k % 16 != 0){ // décaler pour se placer sous la colonne des caractères
        if(k % 4 == 0){
          printf(" ");
        }
        printf("  ");
        k++;
      }
      printf(" ");

      while(j < i){   //affichage de la derniere colonne
        if (tab[j] >= 32 && tab[j] <= 126) {
          printf("%c", tab[j]);
        }
        else{
          printf(".");
        }
        j++;
      }
    }
  }else{
    printf("La section « %s » n'a pas de données à vidanger.\n", nom_section);
  }
}
