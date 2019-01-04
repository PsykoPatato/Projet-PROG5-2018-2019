#ifndef __SECTION_H__
#define __SECTION_H__

void inverse(FILE *f, void *t, int k);

void readSection(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr * shdr);

void printSection(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr * shdr, char *s);

#endif
