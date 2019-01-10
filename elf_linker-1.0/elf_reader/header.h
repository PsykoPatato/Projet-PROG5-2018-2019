#ifndef __HEADER_H__
#define __HEADER_H__

#include <elf.h>
#include "elf32_file.h"

/*lecture par octet dans le fichier et initialisation des champs de la structure*/
void readOctet(FILE *f, void *t, int k, int big);

/*fonction de lecture qui appelle readOctet pour chaque champ*/
void readHeader(FILE *f, Elf32_Ehdr *ehdr);
/*fonction d'affichage*/
void printHeader(Elf32_Ehdr *elf32_ehdr);

#endif
