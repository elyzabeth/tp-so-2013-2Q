/*
 * sockets.h
 *
 *  Created on: Aug 9, 2013
 *      Author: elizabeth
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "paquetes.h"

#define BUFF_SIZE 1024

int32_t crearSocketServidor(uint32_t direccion, int32_t puerto);
int32_t crearSocketCliente (char* direccion, int32_t puerto);
int enviar_buffer (int32_t socketReceptor, char buffer[BUFF_SIZE]);

int lee_socket (int fd, char *Datos, int Longitud);

#endif /* SOCKETS_H_ */
