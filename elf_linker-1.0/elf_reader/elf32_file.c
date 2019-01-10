#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "elf32_file.h"
#include "header.h"
#include "section.h"
#include "symbole.h"
#include "relocation.h"

Elf32_File* initElf32_File(FILE *f){
  Elf32_File *elf32_file = malloc(sizeof(Elf32_File));

  Elf32_Ehdr *ehdr = malloc(sizeof(Elf32_Ehdr));
  elf32_file->ehdr = ehdr;
  assert(ehdr != NULL);
  readHeader(f, ehdr);

  Elf32_Shdr *shdr = malloc(sizeof(Elf32_Shdr) * ehdr->e_shnum);
  elf32_file->shdr = shdr;
  assert(shdr != NULL);
  readTableSection(f, ehdr, shdr);

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
  elf32_file->symb = symb;
  readSymbole(f,elf32_file->ehdr, elf32_file->shdr, elf32_file->symb, elf32_file->iSym);

  return elf32_file;
}

void affichageHeader(Elf32_File *elf32_file){
  printHeader(elf32_file->ehdr);
}

void afficheTableSection(Elf32_File *elf32_file){
    printf("affichage de la table de section\n");
    printTableSection(elf32_file->ehdr, elf32_file->shdr, elf32_file->nom);
}

void affichageSection(FILE *f, Elf32_File* elf32_file, char* char_section){
  printSection(f, elf32_file->ehdr, elf32_file->shdr, elf32_file->nom, char_section);
}

void afficheTableSymbole(FILE *f, Elf32_File *elf32_file){
    printSymbole(f, elf32_file->ehdr, elf32_file->shdr, elf32_file->symb, elf32_file->iSym, elf32_file->nomSym);
}

void afficheReadressage(FILE *f, Elf32_File *elf32_file){
  reloc(f, elf32_file->ehdr, elf32_file->shdr, elf32_file->symb, elf32_file->nom, elf32_file->nomSym);
}
