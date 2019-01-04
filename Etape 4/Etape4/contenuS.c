#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include "contenuS.h"

void contenuS(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr *shdr, int ind, char *nom){

//----------------------lecture de la section -------------------
  fseek(f, shdr[ind].sh_offset, SEEK_SET); //on se place au debut de la section à lire

  int tab[shdr[ind].sh_size]; //tab contiendra les size octets de la section à lire

  //initialisation de tab à 0
  int i=0;
  while(i<shdr[ind].sh_size) {
    tab[i] = 0;
    i++;
  }
  //copie de chaque octet de la section dans tab
  i = 0;
  while(i<shdr[ind].sh_size){
    fread(&tab[i], 1, 1, f);
    i++;
  }



//---------affichage de la section si taille > 0-----------------------------------------
if (shdr[ind].sh_size > 0) {

  printf("Vidange hexadécimale de la section « %s » :", nom);

  i = 0;
  while(i < shdr[ind].sh_size){
    if(i%16 == 0){             //1e colonne
      printf("\n 0x%.8x ", i);
    }
    if(i%4 == 0){     //fin colonne de 4 octets
      printf(" ");
    }
    printf("%.2x", tab[i]);  //affichage de l'octet i


    if(i%16 == 15){  //affichage derniere colonne
      int j = i-15;
      printf(" ");
      while (j < i+1) {
        if (tab[j] >= 32 && tab[j] <= 126) {
          printf("%c", tab[j]);
        }
        else{
          printf(".");
        }
        j++;
      }
    }
    i++;
  } //fin while

  if(i%16 != 15){  //affichage de la derniere colonne si taille non modulo 16
    int j = i - (i%16);
    int k=i;
    while(k%16 != 0){ // décaler pour se placer sous la colonne des caractères
      if(k%4 == 0){
        printf(" ");
      }
      printf("  ");
      k++;
    }
    printf(" ");

    while(j < i){   //affichage de la derniere colonne
      if (tab[j] >= 32 && tab[j] <= 126) {
        printf("%c", tab[j]);
      }
      else{
        printf(".");
      }
      j++;
   }
 }


 }
 //-------------------si taille == 0----pas de données à vidanger!-------------------
 else{
  printf("La section « %s » n'a pas de données à vidanger.\n", nom);
 }


}
