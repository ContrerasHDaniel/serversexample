#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>


#define BUFLEN 512  //cuantos datos 
#define NPACK 10  //cuantos datagramas
#define PORT 9930 //puerto a usar
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
  const struct timeval *tv;
  &tv[BUFLEN];//para guardar el timeval del servidor (recibido)

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
  
  
  //mandamos un datagrama de solicitud
  if (sendto(IdSocket, buf, BUFLEN, 0, (struct sockaddr*)&Servidor, slen)==-1)
    mensajeError("Error en enviar datagrama");

  
  //mandamos n datagramas
  for (i=0; i<NPACK; i++) {
    printf("Recibiendo información del servidor %d\n", i);
    if (recvfrom(IdSocket, &tv, BUFLEN, 0, (struct sockaddr*)&Servidor, &slen)==-1)
      mensajeError("Error en recvfrom()");
  }


  struct timeval now;
  int rc;
  int tmls  = (int) &tv->tv_sec;
  now.tv_sec = tmls/100;
  int tmlm = (int) &tv->tv_sec;
  now.tv_usec = tmlm/1000;
  
  //cambiamos la fecha y hora del sistema.
  rc = settimeofday(&now,0);
  if(rc==0){
    printf("Fecha y hora cambiadas con exito. \n");
  }else{
    printf("settimeofday(): Revise ejecucion con permisos de superusuario, "
        "errno = %d\n",errno);
  }

  //cerramos punto de comunicación
  close(IdSocket);
  return 0;
}