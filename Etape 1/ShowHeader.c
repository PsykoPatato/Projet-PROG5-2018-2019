#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>



void readHeader(int fd, Elf32_Ehdr *ehdr){

   //read the first 4 bytes
   read(fd, ehdr->e_ident, EI_NIDENT);

   if(ehdr->e_ident[0] != ELFMAG0 || ehdr->e_ident[1] != ELFMAG1 || ehdr->e_ident[2] != ELFMAG2 || ehdr->e_ident[3] != ELFMAG3){
      fprintf(stderr, "Le fichier n'est pas de type ELF.\n");
      exit(1);
   }
   //Si le fichier est little endian, rien a faire

     read(fd, &ehdr->e_type, 2);
     read(fd, &ehdr->e_machine, 2);
     read(fd, &ehdr->e_version, 4);
     read(fd, &ehdr->e_entry, 4);
     read(fd, &ehdr->e_phoff, 4);
     read(fd, &ehdr->e_shoff, 4);
     read(fd, &ehdr->e_flags, 4);
     read(fd, &ehdr->e_ehsize, 4);
     read(fd, &ehdr->e_phentsize,4);
     read(fd, &ehdr->e_phnum, 4);
     read(fd, &ehdr->e_shentsize, 4);
     read(fd, &ehdr->e_shnum,  4);
     read(fd, &ehdr->e_shstrndx, 2);

   //Si Big endian, alors lecture en inverse

}

void printHeader(int fd, Elf32_Ehdr *ehdr){
   printf("En-tete ELF:\n");
   //--------------------------------------
   printf(" Magique: ");
   for(int i=0; i<17; i++){
     printf("%.2x ", ehdr->e_ident[i]);
   }
   //-------------------------------------------
   printf("\n Classe:\t");
   switch( ehdr->e_ident[EI_CLASS] ){
     case ELFCLASS32:
       printf("ELF32");
       break;
     case ELFCLASS64:
       printf("ELF64");
       break;
   }
   //--------------------------------------
   printf("\n Donnees:\t");
   if(ehdr->e_ident[EI_DATA] & 1)  //if(ELFDATA2LSB)
     printf("Little Endian");
   else
     printf("Big Endian");

   //--------------------------------
    printf("\n Version:\t");
    if( ehdr->e_ident[EI_VERSION] == EV_CURRENT )
      printf("1 (current)");

    //--------------------------------------
    printf("\n Type:\t");
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
  printf("\n Machine:\t");
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
	case 10240 :
	  printf("ARM \n");
	  break;
	default :
          printf("Inconnu!\n");
   }


    //--------------------------------------
    printf(" Adresse du point d'entree:\t%x\n", ehdr->e_entry);

    //--------------------------------------
    printf(" Debut des en-tetes de programme:\t%x (octets)\n", ehdr->e_phoff);

    //--------------------------------------
    printf(" Debut des en-tetes de section:\t%u  (octets)\n", (unsigned int)ehdr->e_shoff);

    //--------------------------------------
    printf(" Fanions:\t0x%8.8x, Version%x EABI\n", ehdr->e_flags, ehdr->e_flags);

    //--------------------------------------
    printf(" Taille de cet en-tete:\t  %u(octets)\n", ehdr->e_ehsize);

    //--------------------------------------
    printf(" Taille de l'en-tete du programme:\t%x\n", ehdr->e_phentsize);

    //--------------------------------------
    printf(" Nombre d'en-tete du programme:\t%u\n", (unsigned)ehdr->e_phnum);

    //--------------------------------------
    printf(" Taille des en-tetes de section:\t%x  (octets)\n", ehdr->e_shentsize);

    //--------------------------------------
    printf(" Nombre d'en-tete de section:\t%u\n", (unsigned)ehdr->e_shnum);

    //--------------------------------------
    printf(" Table d'indexes des chaines d'en-tete de section:\t%lu\n", (unsigned long)ehdr->e_shstrndx);
}




int main(int argc, char **argv){
   int fd;
   Elf32_Ehdr *ehdr = malloc(sizeof(Elf32_Ehdr));
   assert(ehdr != NULL);
   fd = open(argv[1], O_RDONLY);
   assert(fd >= 0);
   readHeader(fd, ehdr);
   printHeader(fd, ehdr);



  return 0;
}
