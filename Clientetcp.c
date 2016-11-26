//Cliente TCP para pedir hora del sistema al servidor
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/sendfile.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_TO_SEND "fotosfeasdemiex.txt" //archivo a enviar

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
  char size[256];

  //longitud del archivo
  ssize_t len;
  int offset;
  int sent_bytes = 0;
  struct stat file_stat;
  int fd;
  int remain_data;
  
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
  
  fd = open(FILE_TO_SEND, O_RDWR);
  
  //enviamos el tamanio del archivo
  len = send(IdCliente, size, sizeof(size),0);
  if (len < 0)
  {
    /* code */
    printf("Error enviando la solicitud ---");
  }
  
  fstat(fd, &file_stat);
  offset = 0;
  remain_data = file_stat.st_size;

  while(((sent_bytes = sendfile(IdCliente, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
  {
    fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
    remain_data -= sent_bytes;
    fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
  }

  // y cerramos el socket
  close(IdCliente);

  return 0;
}

