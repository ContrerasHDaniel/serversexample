#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFLEN 512 //ysmaño de datos recibidos
#define NPACK 10 //cuantos paquetes
#define PORT 9930  //puerto a usar

//función para manejo de mensaje de error
void mensajeError(char *s)
{
  perror(s);
  exit(1);
}

int main(void)
{
  //estructuras para guardar dastos de cliente y servidor
  struct sockaddr_in Servidor, Cliente;
  int IdSocket, i, slen=sizeof(Cliente);
  char buf[BUFLEN];  //para guardar datos recibidos

  //creamos punto decomunicacion con los datos de UDP, SOCK_DGRAM. y protocolo IP, AF_INET
  if ((IdSocket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    mensajeError("Error en socket!!!");

  //rellenamos con 0's en la estructura de servidor, con tantos 0's como su tamaño. 
  memset((char *) &Servidor, 0, sizeof(Servidor));
  Servidor.sin_family = AF_INET; //ponemos datos de IP y su puerto y aceptamos datos en cualquier dirección ip local
  Servidor.sin_port = htons(PORT);
  Servidor.sin_addr.s_addr = htonl(INADDR_ANY);
  //asociamos el socket ya creado con los datos de la estructura de direccionamiento IP
  if (bind(IdSocket, (struct sockaddr*)&Servidor, sizeof(Servidor))==-1)
      mensajeError("Error en bind");

  //recibimos N paquetes y lo guardamos en el buf, desde el CLIENTE: Cliente
  if (recvfrom(IdSocket, buf, BUFLEN, 0, (struct sockaddr*)&Cliente, &slen)==-1)
    mensajeError("Error en enviar datagrama");
  
  time_t tiempo = time(0);
  struct tm *tlocal = localtime(&tiempo);
  struct tm *tnow = gmtime(&time);
  const struct timeval *tv = {mktime(tlocal), 0};
  &tv[BUFLEN]; //datos
  //mandamos los datos de la hora
  for (i=0; i<NPACK; i++) {
    if (sendto(IdSocket, &tv, BUFLEN, 0, (struct sockaddr*)&Cliente, slen)==-1)
      mensajeError("Error en sendto()");
  }

  //cerramos socket.
  close(IdSocket);
  return 0;
}