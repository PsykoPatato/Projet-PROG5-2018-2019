#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "elf32_file.h"
#include "header.h"
#include "section.h"
#include "contenuS.h"
#include "symbole.h"
#include "relocation.h"

Elf32_File* initElf32_File(FILE *f){
  Elf32_Ehdr *ehdr = malloc(sizeof(Elf32_Ehdr));
  assert(ehdr != NULL);
  readHeader(f, ehdr);

  Elf32_Shdr *shdr = malloc(sizeof(Elf32_Shdr) *ehdr->e_shnum);
  assert(shdr != NULL);
  readSection(f, ehdr, shdr);

  Elf32_File *elf32_file = malloc(sizeof(Elf32_File));
  elf32_file->ehdr = ehdr;
  elf32_file->shdr = shdr;

  int size = elf32_file->shdr[elf32_file->ehdr->e_shstrndx].sh_size;
  char *nom = malloc(size);
  assert(nom != NULL);
  fseek(f, elf32_file->shdr[elf32_file->ehdr->e_shstrndx].sh_offset, SEEK_SET);
  fread(nom, size, 1, f);
  elf32_file->nom = nom;

  int id;
  int i = 0;
   while (i < ehdr->e_shnum && strcmp(".strtab", nom+shdr[i].sh_name)) {
    i++;
  }
  if(i == ehdr->e_shnum){
    printf("indice de « .strtab » non trouvé!\n");
    id = 0;
  }
  else{
    id = i;
  }
  int k = elf32_file->shdr[id].sh_size;
  char *nomSym = malloc(k);
  assert(nomSym != NULL);
  fseek(f, elf32_file->shdr[id].sh_offset, SEEK_SET);
  fread(nomSym, k, 1, f);
  elf32_file->nomSym = nomSym;

  int iSym;
  i = 0;
  while(i < elf32_file->ehdr->e_shnum  &&  elf32_file->shdr[i].sh_type != 2){
    i++;
  }
  if(i == elf32_file->ehdr->e_shnum){
    printf("indice de « .symtab » non trouvé!\n");
    iSym = 0;
  }
  else{
    iSym = i;
  }
  elf32_file->iSym = iSym;


  int taille = elf32_file->shdr[iSym].sh_size / elf32_file->shdr[iSym].sh_entsize;
  Elf32_Sym *symb;
  symb = malloc(sizeof(Elf32_Sym) * taille);
  assert(symb != NULL);
  readSymbole(f,elf32_file->ehdr, elf32_file->shdr, elf32_file->symb, elf32_file->iSym);
  elf32_file->symb = symb;

  return elf32_file;
}

void affichageHeader(Elf32_File *elf32_file){
  printHeader(elf32_file->ehdr);
}

void afficheTableSection(FILE *f, Elf32_File *elf32_file){
    printf("affichage de la table de section\n");
    printSection(f, elf32_file->ehdr, elf32_file->shdr, elf32_file->nom);
}

void affichageSection(FILE *f, Elf32_File* elf32_file, char* char_section){
  int i, v = atoi(char_section);
  int ind;
  char *nomS; //nom de la section à afficher
  if(v > 0 && v < elf32_file->ehdr->e_shnum){ //alors s est le num de la section
   ind = v;
   nomS = elf32_file->nom+elf32_file->shdr[v].sh_name;
  }else{   //s est le nom de la section
   i=0;
   while (i < elf32_file->ehdr->e_shnum && strcmp(char_section, elf32_file->nom+elf32_file->shdr[i].sh_name)) {
     i++;
   }
   if(i == elf32_file->ehdr->e_shnum){
     printf("nom de section invalide!\n");
     ind = 0;
     nomS = "";
     //exit(3);
   }
   else{
     nomS = char_section;
     ind = i;
   }
  }
  contenuS(f, elf32_file->ehdr, elf32_file->shdr, atoi(char_section), nomS);
}

void afficheTableSymbole(FILE *f, Elf32_File *elf32_file){
    printSymbole(f, elf32_file->ehdr, elf32_file->shdr, elf32_file->symb, elf32_file->iSym, elf32_file->nomSym);
}

void afficheReadressage(FILE *f, Elf32_File *elf32_file){
  reloc(f, elf32_file->ehdr, elf32_file->shdr, elf32_file->symb, elf32_file->nom, elf32_file->nomSym);
}
