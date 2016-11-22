//Servidor TCP para conexiones de cliente de tiempo.
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

// Identificador del socket por el que
// se escucha
int IdServidor;

// Identificador del socket con el que
// nos comunicaremos con el cliente
int IdCliente;

//función para manejar interrupciones
void Interrupcion(int Senal)
{
  // Cerramos los sockets
  close(IdServidor);
  close(IdCliente);

  // y salimos
  puts("\nServidor finalizado\n");

  // Relanzamos la señal con su
  // gestor por defecto
  signal(Senal,SIG_DFL);
  raise(Senal);
}

int main()
{
  // Puerto por el que estaremos escuchando
  const int Puerto=IPPORT_USERRESERVED+10;

  // Dirección del servidor
  struct sockaddr_in Servidor;

  // Dirección del cliente que
  // envía la solicitud
  struct sockaddr_in Cliente;
  // longitud de la dirección
  int LongCliente=sizeof(Cliente);

  // Para obtener la fecha y hora y 
  // darle formato
  time_t Ahora;
  char *FechaHora;

  // Interceptamos la señal de
  // interrupción del proceso
  signal(SIGINT,Interrupcion);

  // Preparamos la estructura que 
  // identificará al servidor
  Servidor.sin_family=AF_INET; //
  Servidor.sin_addr.s_addr=INADDR_ANY; //cualquier dirección local
  Servidor.sin_port=htons(Puerto); //convertimos puerto host en formato de red

  // Creamos el socket del servidor, de tipo TCP, SOCK_STREAM
  IdServidor=socket(PF_INET,SOCK_STREAM,0);
  // y comprobamos que es correcto
  if(IdServidor==-1) {
    puts("Fallo en la creación del socket");
    exit(-1);
  }

  // Asociamos el socket con la dirección
  // y puerto indicados en la estructura
  if(bind(IdServidor,(struct sockaddr*)&Servidor,sizeof(struct sockaddr_in))==-1) {
    puts("Fallo en la llamada a bind()");
    exit(-1);
  }

  // Ponemos el socket a la escucha con 5 conexiones
  if(listen(IdServidor,5)==-1) {
    puts("Fallo en la llamada a listen()");
    exit(-1);
  }

  while(1) { // repetir mientras no se interrumpa
    // esperamos a que llegue una solicitud, para aceptarla
    IdCliente=accept(IdServidor,(struct sockaddr*)&Cliente,&LongCliente);

    // Comprobamos que todo va bien
    if(IdCliente==-1) {
      puts("Fallo en la llamada a accept()");
      exit(-1);
    }

    // Indicamos el origen de la solicitud, convirtiendo de formato de red a host
    printf("Solicitud desde %s\n", inet_ntoa(Cliente.sin_addr));

    // Le enviamos al cliente la información de la hora actual
    Ahora=time(NULL);
    FechaHora=ctime(&Ahora);

    //enviar información de hora
    send(IdCliente,FechaHora,strlen(FechaHora)+1,0);

    // Cerramos la conexión con este cliente
    close(IdCliente);
  }

  return 0; //terminamos función principal.
}
