#include <elf.h>

typedef struct Elf32_File{
  Elf32_Ehdr *ehdr;
  Elf32_Shdr *shdr;
  Elf32_Sym *symb;
  char *nomSym;
  char *nom;
  int iSym;
}Elf32_File;

Elf32_File* initElf32_File(FILE *f);
void affichageHeader(Elf32_File* elf32_file);
void afficheTableSection(FILE *f, Elf32_File *elf32_file);
void afficheSection(FILE *f, Elf32_File *elf32_file, char* char_section);
void afficheTableSymbole(FILE *f, Elf32_File *elf32_file);
void afficheTableReadressage(FILE *f, Elf32_File *elf32_file);
