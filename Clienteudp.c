#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFLEN 512  //cuantos datos 
#define NPACK 1     //cuantos datagramas
#define PORT 9930   //puerto a usar
#define SRV_IP "127.0.0.1" //Ip del servidor

//función para menejo de errores
void mensajeError(char *s)
{
  perror(s);
  exit(1);
}

int main(void)
{
  //estructura para el servidor
  struct sockaddr_in Servidor;
  int IdSocket, i, slen=sizeof(Servidor);
  char buf[BUFLEN]; //datos

  //creamos punto de comunicación, de tipo UDP, SOCK_DGRAM
  if ((IdSocket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    mensajeError("Error en socket!!!");

  //rellenamos con 0 a la estructura de Servidor (inicalizarla em ceros),
  memset((char *) &Servidor, 0, sizeof(Servidor));

  //asignamos datos a estructura, con protocolo IP y puerto convertido.
  Servidor.sin_family = AF_INET;
  Servidor.sin_port = htons(PORT);

  //asignamos dirección IP convertida
  if (inet_aton(SRV_IP, &Servidor.sin_addr)==0) {
    fprintf(stderr, "Error en inet_aton()\n");
    exit(1);
  }

  //mandamos 1 solo datagrama de solicitud
  // Aqui habia un ciclo for
  printf("Enviando solicitud al servidor ... \n");
  sprintf(buf, "Datagrama %d\n", NPACK);
  //mandamos buf con la cadena Datagrama 0.1.2...", al servidor
  if (sendto(IdSocket, buf, BUFLEN, 0, (struct sockaddr*)&Servidor, slen)==-1)
    mensajeError("Error en enviar datagrama");

  //recibimos el paquete con la fecha local del servidor
  if (recvfrom(IdSocket, buf, BUFLEN, 0, (struct sockaddr*)&Servidor, &slen)==-1)
       mensajeError("Error en recvfrom()");

  printf("Recibiendo datos del datagrama desde %s:%d\nData: %s\n", 
  inet_ntoa(Servidor.sin_addr), ntohs(Servidor.sin_port), buf);

  //cerramos punto de comunicación
  close(IdSocket);
  return 0;
}