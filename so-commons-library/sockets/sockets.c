/*
 * sockets.c
 *
 *  Created on: Aug 9, 2013
 *      Author: elizabeth
 */

#include "sockets.h"


int32_t crearSocketServidor(uint32_t direccion, int32_t puerto) {

	struct sockaddr_in socketInfo;
	int optval = 1;

	int32_t socketEscucha;
	// Crear un socket:
	// AF_INET: Socket de internet IPv4
	// SOCK_STREAM: Orientado a la conexion, TCP
	// 0: Usar protocolo por defecto para AF_INET-SOCK_STREAM: Protocolo TCP/IPv4
	if ((socketEscucha = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("crearSocketServidor: socket");
		return EXIT_FAILURE;
	}

	// Hacer que el SO libere el puerto inmediatamente luego de cerrar el socket.
	setsockopt(socketEscucha, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	socketInfo.sin_family = AF_INET;
	socketInfo.sin_addr.s_addr = direccion; //Notar que aca no se usa inet_addr()
	socketInfo.sin_port = htons(puerto);

	// Vincular el socket con una direccion de red almacenada en 'socketInfo'.
	if (bind(socketEscucha, (struct sockaddr*) &socketInfo, sizeof(socketInfo)) != 0) {
		perror("crearSocketServidor: Error al bindear socket escucha");
		return EXIT_FAILURE;
	}

	// Escuchar nuevas conexiones entrantes.
	if (listen(socketEscucha, 10) != 0) {

		perror("crearSocketServidor: Error al poner a escuchar socket");
		return EXIT_FAILURE;

	}

	return socketEscucha;

}

int32_t crearSocketCliente (char* direccion, int32_t puerto) {
	int unSocket;
	struct sockaddr_in socketInfo;

	// Crear un socket:
	// AF_INET: Socket de internet IPv4
	// SOCK_STREAM: Orientado a la conexion, TCP
	// 0: Usar protocolo por defecto para AF_INET-SOCK_STREAM: Protocolo TCP/IPv4
	if ((unSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error al crear socket");
		return EXIT_FAILURE;
	}

	socketInfo.sin_family = AF_INET;
	socketInfo.sin_addr.s_addr = inet_addr(direccion);
	socketInfo.sin_port = htons(puerto);

	// Conectar el socket con la direccion 'socketInfo'.
	if (connect(unSocket, (struct sockaddr*) &socketInfo, sizeof(socketInfo)) != 0) {
		perror("Error al conectar socket");
		return EXIT_FAILURE;
	}

	return unSocket;
}


int recv_variable(int32_t socketReceptor, void* buffer) {

	t_header header;
	int bytesRecibidos;

	// Primero: Recibir el header para saber cuanto ocupa el payload.
	if (recv(socketReceptor, &header, sizeof(header), MSG_WAITALL) <= 0)
		return -1;

	// Segundo: Alocar memoria suficiente para el payload.
	buffer = malloc(header.length);

	// Tercero: Recibir el payload.
	if((bytesRecibidos = recv(socketReceptor, buffer, header.length, MSG_WAITALL)) < 0){
		free(buffer);
		return -1;
	}

	return bytesRecibidos;

}

int enviar_buffer (int32_t socketReceptor, char buffer[BUFF_SIZE]) {

	if (send(socketReceptor, buffer, strlen(buffer), 0) >= 0) {
		printf("Datos enviados!\n");

	} else {
		perror("Error al enviar datos. Server no encontrado.\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


int recibir_buffer () {

	return EXIT_SUCCESS;
}


/*
* Lee datos del socket. Supone que se le pasa un buffer con hueco
*	suficiente para los datos. Devuelve el numero de bytes leidos o
* 0 si se cierra fichero o -1 si hay error.
*/
int lee_socket (int fd, char *Datos, int Longitud)
{
	int Leido = 0;
	int Aux = 0;

	/*
	* Comprobacion de que los parametros de entrada son correctos
	*/
	if ((fd == -1) || (Datos == NULL) || (Longitud < 1))
		return -1;

	/*
	* Mientras no hayamos leido todos los datos solicitados
	*/
	while (Leido < Longitud)
	{
		Aux = read (fd, Datos + Leido, Longitud - Leido);
		if (Aux > 0)
		{
			/*
			* Si hemos conseguido leer datos, incrementamos la variable
			* que contiene los datos leidos hasta el momento
			*/
			Leido = Leido + Aux;
		}
		else
		{
			/*
			* Si read devuelve 0, es que se ha cerrado el socket. Devolvemos
			* los caracteres leidos hasta ese momento
			*/
			if (Aux == 0)
				return Leido;
			if (Aux == -1)
			{
				/*
				* En caso de error, la variable errno nos indica el tipo de error.
				* El error EINTR se produce si ha habido alguna
				* interrupcion del sistema antes de leer ningun dato. No
				* es un error realmente.
				* El error EGAIN significa que el socket no esta disponible
				* de momento, que lo intentemos dentro de un rato.
				* Ambos errores se tratan con una espera de 100 microsegundos
				* y se vuelve a intentar.
				* El resto de los posibles errores provocan que salgamos de
				* la funcion con error.
				*/
				switch (errno)
				{
					case EINTR:
					case EAGAIN:
						usleep (100);
						break;
					default:
						return -1;
				}
			}
		}
	}

	/*
	* Se devuelve el total de los caracteres leidos
	*/
	return Leido;
}
