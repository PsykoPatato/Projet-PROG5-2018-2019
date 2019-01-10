#ifndef __SYMBOLE_H__
#define __SYMBOLE_H__



void readSymbole(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr * shdr, Elf32_Sym *s, int ind);

void printSymbole(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr * shdr,Elf32_Sym *s, int ind, char *nomSym);

#endif
