#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <malloc.h>
#include "saca.h"



int main(int argc, char const *argv[])
{
  struct sockadr_in adresse;
  int sock = 0, valread;
  struct sockadr_in serv_adr;
  char commandBuffer[1024];  
  char donnee[1024];
  char id[1024];
  char command[1024];
  
  sock = socket(AF_INET,SOCK_STREAM,0);
  memset(&serv_adr, '0', sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_port = htons(PORTC);
	
  inet_pton(AF_INET, "130.0.0.1", &serv_adr.sin_adr);	
  
  if(connect(sock, (struct sockadr *)&serv_adr, sizeof(serv_adr)) < 0)
  {
    printf("\n Connection Failed");
    return -1;
  }
  
  printf("Bienvennue a SACA, veuillez entrez votre ID forme de 3 characteres: \n");
  scanf("%s", id);
  while(strlen(id) !=3)
  {
    printf("ID icompatible, veuillez entrer une nouvelle valeur:\n");
    scanf("%s", id);
  }
  printf("Bonne ID:%s\n",id);
  printf("Vous devez envoyer le nom de l'avion forme de 5 characteres exemple:SA500 suivi des commandes: \n");
  printf("-a=700 => changer altitude de l'avion a 700\n");
  printf("-v=800 => changer vitesse de l'avion a 800\n");
  printf("-c=150 => changer cap de l'avion de a 150\n");
  printf("-l     => liberer avion \n");

  while(1)
  {
    scanf("%s", command);
    strcpy(commandBuffer, id);
    strcat(commandBuffer, "/");
    strcat(commandBuffer, command);
    send(sock, commandBuffer, 1024, 0);
    read(sock, donnee, 1024);
    printf("%s\n", donnee);
  }

}

