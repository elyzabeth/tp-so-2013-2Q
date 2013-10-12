/*
 * NivelMain.c
 *
 *  Created on: 22/09/2013
 *      Author: elyzabeth
 */

#include "NivelMain.h"


int main (int argc, char**argv) {

	// Correr tests
	if (argc > 1 && strcmp(argv[1], "-test")==0)
		return correrTest();

	// Registro signal and signal handler
	signal(SIGINT, signal_callback_handler);
	signal(SIGQUIT, signal_callback_handler);
	signal(SIGUSR1, signal_callback_handler);
	signal(SIGTERM, signal_callback_handler);

	inicializarNivel();
	principal ();
	//while(1);
	finalizarNivel();

	return EXIT_SUCCESS;
}



void principal () {
		int id_proceso = getpid();
		int sock = -1;
		fd_set master;
		fd_set read_fds;
		FD_ZERO(&master);
		FD_ZERO(&read_fds);
		int max_desc = 0;

		log_info(LOGGER,"************** Iniciando Nivel '%s' (PID: %d) ***************\n", NOMBRENIVEL, id_proceso);
		// Conectar con proceso Plataforma
		conectar(configNivelPlataformaIp(), configNivelPlataformaPuerto(), &sock);
		if (enviarMSJNuevoNivel(sock) != EXITO){
			log_error(LOGGER, "ERROR en conexion con Plataforma");
			finalizarNivel();
			exit(EXIT_FAILURE);
		}

		agregar_descriptor(sock, &master, &max_desc);

		pthread_create (&idHiloInterbloqueo, NULL, (void*) interbloqueo, NULL);

		while(1);
		//simulacroJuego ();
		//ejemploGui();

		return;
}






