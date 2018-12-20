#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>

//static Elf32_Word SHT_ARM_DEBUGOVERLAY = 0x70000004;
//static Elf32_Word SHT_ARM_OVERLAYSECTION = 0x70000005;
struct Shdr_donnees_vecteur{
  int taille;
  Elf32_Shdr* donnees;
};

typedef struct Shdr_donnees_vecteur *Shdr_vecteur;



Shdr_vecteur readHeaderSection(int fd){
  Elf32_Ehdr *ehdr = malloc(sizeof(Elf32_Ehdr));
  assert(ehdr != NULL);
  read(fd, &ehdr->e_ident, EI_NIDENT);
  read(fd, &ehdr->e_type, sizeof(ehdr->e_type));
  read(fd, &ehdr->e_machine, sizeof(ehdr->e_machine));
  read(fd, &ehdr->e_version, sizeof(ehdr->e_version));
  read(fd, &ehdr->e_entry, sizeof(ehdr->e_entry));
  read(fd, &ehdr->e_phoff, sizeof(ehdr->e_phoff));
  read(fd, &ehdr->e_shoff, sizeof(ehdr->e_shoff));
  read(fd, &ehdr->e_flags, sizeof(ehdr->e_flags));
  read(fd, &ehdr->e_ehsize, sizeof(ehdr->e_ehsize));
  read(fd, &ehdr->e_phentsize, sizeof(ehdr->e_phentsize));
  read(fd, &ehdr->e_phnum, sizeof(ehdr->e_phnum));
  read(fd, &ehdr->e_shentsize, sizeof(ehdr->e_shentsize));
  read(fd, &ehdr->e_shnum, sizeof(ehdr->e_shnum));
  read(fd, &ehdr->e_shstrndx, sizeof(ehdr->e_shstrndx));

  Shdr_vecteur vecteur = malloc(sizeof(struct Shdr_donnees_vecteur));
  vecteur->donnees = malloc(sizeof(Elf32_Shdr) * ehdr->e_shnum);
  //vecteur->taille = ehdr->e_shnum;
  vecteur->taille = 15;

  Elf32_Shdr *shdr;
  shdr = malloc(sizeof(Elf32_Shdr));
  //int offset = ehdr->e_shoff;
  int offset = 760;
  //printf("nombre : %d\n", ehdr->e_shentsize);
  //printf("shnum : %x", ehdr->e_shentsize);
  for(int i = 0; i < vecteur->taille; i++){
    lseek(fd, offset, SEEK_SET);
    read(fd, &shdr->sh_name, sizeof(shdr->sh_name));
    read(fd, &shdr->sh_type, sizeof(shdr->sh_type));
    read(fd, &shdr->sh_flags, sizeof(shdr->sh_flags));
    read(fd, &shdr->sh_addr, sizeof(shdr->sh_addr));
    read(fd, &shdr->sh_offset, sizeof(shdr->sh_offset));
    read(fd, &shdr->sh_size, sizeof(shdr->sh_size));
    read(fd, &shdr->sh_link, sizeof(shdr->sh_link));
    read(fd, &shdr->sh_info, sizeof(shdr->sh_info));
    read(fd, &shdr->sh_addralign, sizeof(shdr->sh_addralign));
    read(fd, &shdr->sh_entsize, sizeof(shdr->sh_entsize));
    vecteur->donnees[i] = *shdr;
    offset += 40;
    //offset += ehdr->e_shentsize;
  }
  return vecteur;
}

void printHeaderSection(int fd, Shdr_vecteur vecteur){

  printf("[Nr]")

  for(int i = 0; i < vecteur->taille; i++){
    Elf32_Shdr *shdr = vecteur->donnees + i;

    printf("Nom : %d\n", shdr->sh_name);
    printf("Type : ");
    switch(shdr->sh_type){
      case SHT_NULL :
        printf("NULL\n");
        break;
      case SHT_PROGBITS :
        printf("PROGBITS\n");
        break;
      case SHT_SYMTAB :
        printf("SYMTAB\n");
        break;
      case SHT_STRTAB :
        printf("STRTAB\n");
        break;
      case SHT_RELA :
        printf("RELA\n");
        break;
      case SHT_HASH :
        printf("HASH\n");
        break;
      case SHT_DYNAMIC :
        printf("DYNAMIC\n");
        break;
      case SHT_NOTE :
        printf("NOTE\n");
        break;
      case SHT_NOBITS :
        printf("NOBITS\n");
        break;
      case SHT_REL :
        printf("REL\n");
        break;
      case SHT_DYNSYM :
        printf("DYNSYM\n");
        break;
      case SHT_LOPROC :
        printf("LOPROC\n");
        break;
      case SHT_HIPROC :
        printf("HIPROC\n");
        break;
      case SHT_LOUSER :
        printf("LOUSER\n");
        break;
      case SHT_HIUSER :
        printf("HIUSER\n");
        break;
      case SHT_ARM_EXIDX :
        printf("ARM_EXIDX\n");
        break;
      case SHT_ARM_PREEMPTMAP :
        printf("ARM_PREEMPTMAP\n");
        break;
      case SHT_ARM_ATTRIBUTES :
        printf("ARM_ATTRIBUTES\n");
        break;
      case 0x70000004 :
        printf("ARM_DEBUGOVERLAY\n");
        break;
      case 0x70000005 :
        printf("ARM_OVERLAYSECTION\n");
        break;
      }
      //PRINTF(FLAGS)
      printf("Adr : %x\n", shdr->sh_addr);
      printf("Decalage : %x\n", shdr->sh_offset);
      printf("Taille : %x\n", shdr->sh_size);
      printf("ES : %x\n", shdr->sh_entsize);
      printf("LN : %x\n", shdr->sh_link);
      printf("Info : %x\n", shdr->sh_info);
      printf("AdrAlign : %x\n", shdr->sh_addralign);
  }
}

int main(int argc, char** argv){
  int fd;
  fd = open(argv[1], O_RDONLY);
  assert(fd >= 0);
  //readHeaderSection(fd);
  Shdr_vecteur vecteur = readHeaderSection(fd);
  printHeaderSection(fd, vecteur);
}
