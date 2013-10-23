//include comunicacion.h
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <time.h>
#include <netinet/tcp.h>

#include "../tads/tad_personaje.h"

#define ERROR		    0
#define EXITO		    1
#define WARNING         2

#define OTRO			9
#define NUEVO_PERSONAJE			0x10
#define NUEVO_NIVEL				0x11
#define PERSONAJE_CONECTADO 	0x12
#define NIVEL_CONECTADO			0x13
#define CONECTAR_NIVEL 			0x14
#define SOLICITUD_UBICACION		0x15
#define SOLICITUD_RECURSO		0x16
#define RECURSO_CONCEDIDO		0x17
#define RECURSO_DENEGADO		0x18
#define TURNO_CONCEDIDO			0x19
#define FINALIZAR				0x20
#define CAMBIOS_CONFIGURACION	0x21


#pragma pack(1)
typedef struct header_s
{
  char id[16];
  char tipo;
  int  largo_mensaje;

}header_t;
#pragma pack(0)

header_t* crearHeader();
void initHeader(header_t* header);
int enviar(int sock, char *buffer, int tamano);
int recibir(int sock, char *buffer, int tamano);
int conectar(char ip[15+1], int puerto, int *sock);
int aceptar_conexion(int *listener, int *nuevo_sock);
int recibir_header(int sock, header_t *header, fd_set *master/*por si se desconecta*/, int *seDesconecto);
int recibir_personaje(int sock, t_personaje *personaje, fd_set *master, int *seDesconecto);
int aceptar_conexion(int *listener, int *nuevoSock);
int agregar_descriptor(int desc, fd_set *listaDesc, int *maxDesc);
int quitar_descriptor(int desc, fd_set *listaDesc, int *maxDesc);
int crear_listener(int puerto, int *listener);
void genId(char idMsg[]);

void senialMurioHijo(int senial);
//void senialSuspendido(int senial);

