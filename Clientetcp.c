//Cliente TCP para pedir hora del sistema al servidor
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
  // Puerto donde esperamos encontrar el
  // servidor a la escucha
  const int Puerto=IPPORT_USERRESERVED+10;

  // Dirección del servidor
  struct sockaddr_in Servidor;

  // Identificador del socket
  int IdCliente;

  // Para leer los datos
  char FechaHora[256];

  // Preparamos la estructura con la
  // dirección y puerto del servidor
  Servidor.sin_family=AF_INET; //Protocolo IP
  inet_aton("127.0.0.1", &Servidor.sin_addr); //convertir dirección IP
  Servidor.sin_port=htons(Puerto);  //convertir puerto host a puerto red

  // Creamos el socket para comunicarnos
  IdCliente=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
  // y comprobamos que es válido
  if(IdCliente==-1) {
    puts("Fallo en la llamada a socket()");
    exit(-1);
  }

  // Conectamos comprobando un posible fallo
  if(connect(IdCliente,(struct sockaddr*)&Servidor,sizeof(Servidor))==-1) {
    puts("Fallo en la llamada a connect()");
    exit(-1);
  }
  //mostramos mesaje en pantalla
  printf("Estableciendo conexión con servidor: %s al puerto %d\n",\
    inet_ntoa(Servidor.sin_addr),ntohs(Servidor.sin_port) );

  // Leemos los datos que nos 
  // facilita el servidor
  recv(IdCliente,FechaHora,256,0);

  // los mostramos
  printf("Dato recibido: %s", FechaHora);

  // y cerramos el socket
  close(IdCliente);

  return 0;
}

