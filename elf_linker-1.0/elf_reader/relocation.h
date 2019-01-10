#ifndef __RELOCATION_H__
#define __RELOCATION_H__


void reloc(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr * shdr, Elf32_Sym *s, char *nomSec, char *nomSym);



#endif
