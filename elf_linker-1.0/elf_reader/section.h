#ifndef __SECTION_H__
#define __SECTION_H__

void inverse(FILE *f, void *t, int k);
char *type(int val);

void readTableSection(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr *shdr);
void printTableSection(Elf32_Ehdr *ehdr, Elf32_Shdr *shdr, char *nom);
void printSection(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr *shdr, char* nom, char* char_section);

#endif
