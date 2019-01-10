#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>

void readOctet(FILE *f, void *t, int k, int big){
  int  i, c;
  char s[k];
  if(big == 0){
    fread(t, k, 1, f);
  }
  else {
    for(i=0; i<k; i++){
      c = fgetc(f);
      s[i] = c;
    }

    for(i=k-1; i>=0; i--){
      *((char *)t++) = s[i];
    }
  }
}

void printHeader(Elf32_Ehdr *ehdr){
   printf("En-tete ELF:\n");
   //--------------------------------------
   printf(" Magique: ");
   for(int i = 0; i < 16; i++){
     printf("%.2x ", ehdr->e_ident[i]);
   }
   //-------------------------------------------
   printf("\n Classe:\t");
   switch(ehdr->e_ident[EI_CLASS] ){
     case ELFCLASS32:
       printf("ELF32");
       break;
     case ELFCLASS64:
       printf("ELF64");
       break;
   }
   //--------------------------------------
   printf("\n Donnees:\t");
   if(ehdr->e_ident[EI_DATA] == ELFDATA2LSB){
     printf("Little Endian");
   }else if(ehdr->e_ident[EI_DATA] == ELFDATA2MSB){
     printf("Big Endian");
   }
   //--------------------------------
   printf("\n Version:\t");
   if(ehdr->e_ident[EI_VERSION] == EV_CURRENT)
    printf("1 (current)");
   //--------------------------------------
   printf("\n Type:\t ");
   switch(ehdr->e_type){
	    case ET_REL :
	      printf("REL (Fichier de readressage) \n");
	      break;
	    case ET_EXEC :
	      printf("Exec (Fichier d'execution) \n");
	      break;
	    case ET_DYN :
        printf("DYN (Objet Partage) \n");
	      break;
	    case ET_CORE :
	      printf("CORE (Fichier Core) \n");
	      break;
	    case ET_LOPROC | ET_HIPROC :
	      printf("PROC (Fichier spécifique au Processeur) \n");
	      break;
      default:
        printf("pas de type pour le fichier!");
	      break;
    }
    //--------------------------------------
    printf(" Machine:\t ");
    switch(ehdr->e_machine){
      case EM_M32 :
        printf("AT&T WE 32100 \n");
        break;
      case EM_SPARC :
        printf("SPARC \n");
        break;
      case EM_386 :
        printf("Intel 80386 \n");
        break;
      case EM_68K :
        printf("Motorola 68000 \n");
        break;
      case EM_88K :
        printf("Motorola 88000 \n");
        break;
      case EM_860 :
        printf("Intel 80860 \n");
        break;
      case EM_MIPS :
        printf("MIPS RS3000 \n");
        break;
      case EM_ARM :
        printf("ARM \n");
        break;
      default :
        printf("Inconnu!\n");
   }
   //--------------------------------------------------------
   printf(" VERSION ABI: \t %u\n", (unsigned)ehdr->e_ident[EI_ABIVERSION]);
   //-------------------------------------------------------------
   printf(" VERSION: \t 0x%x\n", ehdr->e_version);
   //--------------------------------------------------------
   printf(" OS/ABI: \t");
   switch(ehdr->e_ident[EI_OSABI]){
      case ELFOSABI_NONE:
          printf("UNIX System V ABI");
          break;
      case ELFOSABI_HPUX:
          printf("HP-UX");
          break;
      case ELFOSABI_NETBSD:
          printf("NetBSD. ");
          break;
      case ELFOSABI_GNU:
          printf("Object uses GNU ELF extensions. ");
          break;
      case ELFOSABI_SOLARIS:
          printf("Sun Solaris. ");
          break;
      case ELFOSABI_AIX:
          printf("IBM AIX. ");
          break;
      case ELFOSABI_IRIX:
          printf("SGI Irix. ");
          break;
      case ELFOSABI_FREEBSD:
          printf("FreeBSD. ");
          break;
      case ELFOSABI_TRU64:
          printf("Compaq TRU64 UNIX. ");
          break;
      case ELFOSABI_MODESTO:
          printf("Novell Modesto. ");
          break;
      case ELFOSABI_OPENBSD:
          printf("OpenBSD. ");
          break;
      case ELFOSABI_ARM_AEABI:
          printf("ARM EABI");
          break;
      case ELFOSABI_ARM:
          printf("/* ARM");
          break;
      case ELFOSABI_STANDALONE:
          printf("Standalone (embedded) application");
          break;
   }
   //--------------------------------------
   printf(" \n Adresse du point d'entree:\t0x%x\n", ehdr->e_entry);
   //--------------------------------------
   printf(" Debut des en-tetes de programme:\t%u (octets)\n", (unsigned)ehdr->e_phoff);
   //--------------------------------------
   printf(" Debut des en-tetes de section:\t%u (octets)\n", (unsigned)ehdr->e_shoff);
   //--------------------------------------
   printf(" Fanions:\t0x%x, Version%d EABI\n", ehdr->e_flags, *((char *)&ehdr->e_flags+3));
   //--------------------------------------
   printf(" Taille d'une entree de la table d'en-tete:\t%u(octets)\n", (unsigned)ehdr->e_phentsize);
   //--------------------------------------
   printf(" Taille de l'en-tete du programme:\t%u\n",  (unsigned)ehdr->e_ehsize);
   //--------------------------------------
   printf(" Nombre d'en-tete du programme:\t%u\n", (unsigned) ehdr->e_phnum);
   //--------------------------------------
   printf(" Taille des en-tetes de section:\t%u  (octets)\n", (unsigned) ehdr->e_shentsize);
   //--------------------------------------
   printf(" Nombre d'en-tete de section:\t%u\n",  (unsigned) ehdr->e_shnum);
   //--------------------------------------
   printf(" Table d'indexes des chaines d'en-tete de section:\t%u\n", ehdr->e_shstrndx);
}

void readHeader(FILE *f, Elf32_Ehdr *ehdr){
   fread(ehdr->e_ident, 16, 1, f);

   if(ehdr->e_ident[0] != ELFMAG0 || ehdr->e_ident[1] != ELFMAG1 || ehdr->e_ident[2] != ELFMAG2 || ehdr->e_ident[3] != ELFMAG3){
      fprintf(stderr, "Le fichier n'est pas de type ELF.\n");
      exit(1);
   }

   int big = 0;
   if(ehdr->e_ident[EI_DATA] == ELFDATA2MSB){
     big = 1;
   }
   readOctet(f, &ehdr->e_type, sizeof(ehdr->e_type), big);
   readOctet(f, &ehdr->e_machine, sizeof(ehdr->e_machine), big);
   readOctet(f, &ehdr->e_version, sizeof(ehdr->e_version), big);
   readOctet(f, &ehdr->e_entry, sizeof(ehdr->e_entry), big);
   readOctet(f, &ehdr->e_phoff, sizeof(ehdr->e_phoff), big);
   readOctet(f, &ehdr->e_shoff, sizeof(ehdr->e_shoff), big);
   readOctet(f, &ehdr->e_flags, sizeof(ehdr->e_flags), big);
   readOctet(f, &ehdr->e_ehsize, sizeof(ehdr->e_ehsize), big);
   readOctet(f, &ehdr->e_phentsize, sizeof(ehdr->e_phentsize), big);
   readOctet(f, &ehdr->e_phnum, sizeof(ehdr->e_phnum), big);
   readOctet(f, &ehdr->e_shentsize, sizeof(ehdr->e_shentsize), big);
   readOctet(f, &ehdr->e_shnum, sizeof(ehdr->e_shnum), big);
   readOctet(f, &ehdr->e_shstrndx, sizeof(ehdr->e_shstrndx), big);
}
