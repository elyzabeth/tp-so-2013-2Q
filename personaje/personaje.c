/*
 * personaje.c
 *
 *  Created on: Sep 24, 2013
 *      Author: elizabeth
 */

#include "personaje.h"



int main (int argc, char *argv[]) {

	signal(SIGINT, per_signal_callback_handler);
	signal(SIGQUIT, per_signal_callback_handler);
	signal(SIGUSR1, per_signal_callback_handler);
	signal(SIGTERM, per_signal_callback_handler);

	inicializarPersonaje();

	// TODO agregar logica del personaje
	principal(argc, argv);

	finalizarPersonaje();

	return EXIT_SUCCESS;
}


int principal(int argc, char *argv[]) {

	int id_proceso;

	id_proceso = getpid();
	system("clear");

	log_info(LOGGER,"************** Iniciando Personaje '%s' (PID: %d) ***************\n", personaje.nombre, id_proceso);

	levantarHilosxNivel();
while(1); // TODO QUITAR ESTOOO!!!

	return 0;
}

void* personajexNivel (t_hilo_personaje *hiloPxN) {
	int sock = -1;
	header_t header;
	fd_set master;
	fd_set read_fds;
	int max_desc = 0;
	int i, ret;
	int fin = false;

	log_info(LOGGER, "Hilo Personaje para objetivos del nivel %s Iniciado", hiloPxN->personaje.nivel);
sleep(1);

	/***************** ME CONECTO Y ARMO MENSAJE DE PRESENTACION *******/
	log_info(LOGGER,"************** CONECTANDOSE  ***************\n");
	conectar(personaje.ip_orquestador, personaje.puerto_orquestador, &sock);

	FD_ZERO(&master);
	// Agrego descriptor del Pipe con Nivel.
	agregar_descriptor(hiloPxN->fdPipe[0], &master, &max_desc);

	agregar_descriptor(sock, &master, &max_desc);

	if (enviarMsjNuevoPersonaje(sock) != EXITO)
	{
		log_error(LOGGER,"Error al enviar header NUEVO_PERSONAJE\n\n");
		fin = true;
		//return WARNING;
	}


	while(!fin)
	{
		FD_ZERO (&read_fds);
		read_fds = master;

		ret = select(max_desc+1, &read_fds, NULL, NULL, NULL);

		if(ret == -1) {
			printf("Personaje %s '%c': ERROR en select.", hiloPxN->personaje.nombre, hiloPxN->personaje.id);
			sleep(1);
			continue;
		}

		if (ret > 0) {
			for(i = 0; i <= max_desc; i++)
			{

				if (FD_ISSET(i, &read_fds))
				{
					if( i == hiloPxN->fdPipe[0])
					{
						header_t header;
						log_info(LOGGER, "Personaje '%c': Recibo mensaje desde Main por Pipe", hiloPxN->personaje.id);
						read (hiloPxN->fdPipe[0], &header, sizeof(header_t));

						log_debug(LOGGER, "Personaje '%c': mensaje recibido '%d'", hiloPxN->personaje.id, header.tipo);
						if (header.tipo == FINALIZAR) {
							log_debug(LOGGER, "Personaje '%c': '%d' ES FINALIZAR", hiloPxN->personaje.id, header.tipo);
							fin = true;
							break;
						}

					} else {

						memset(&header, '\0', sizeof(header_t));
						recibirHeaderNuevoMsj(sock, &header);

						switch (header.tipo) /*recibo estado */
						{
							case PERSONAJE_CONECTADO: log_info(LOGGER,"Personaje Conectado");
								enviarInfoPersonaje(sock);
								break;

							case OTRO: log_info(LOGGER, "otro");
								break;

						}


					}
				}
			}

		}
	}

	log_info(LOGGER, "FINALIZANDO Hilo Personaje '%c' Nivel %s\n", hiloPxN->personaje.id, hiloPxN->personaje.nivel);

	pthread_exit(NULL);
}

/**
 * @NAME: inicializarPersonaje
 * @DESC: Inicializa todas las variables y estructuras necesarias para el proceso personaje
 */
void inicializarPersonaje() {
	levantarArchivoConfiguracionPersonaje();
	// TODO agregar inicializaciones necesarias
	LOGGER = log_create(configPersonajeLogPath(), "PERSONAJE", configPersonajeLogConsola(), configPersonajeLogNivel() );
	log_info(LOGGER, "INICIALIZANDO PERSONAJE '%s' ", configPersonajeNombre());

	strcpy(personaje.nombre, configPersonajeNombre());
	strcpy(personaje.ip_orquestador, configPersonajePlataformaIp());
	personaje.puerto_orquestador = configPersonajePlataformaPuerto();
	VIDAS = configPersonajeVidas();
	REINTENTOS = 0;
	planDeNiveles = configPersonajePlanDeNiveles();
	listaHilosxNivel = list_create();

	buffer_header = calloc(1,sizeof(header_t));
}

/**
 * @NAME: finalizarPersonaje
 * @DESC: Finaliza todas las variables y estructuras que fueron creadas para el proceso personaje
 */
void finalizarPersonaje() {
	log_info(LOGGER, "FINALIZANDO PERSONAJE\n");

	// TODO Bajar Hilos
	// matarHilos()...

	list_destroy_and_destroy_elements(listaHilosxNivel, (void*)destruirHiloPersonaje);
	destruirConfigPersonaje();
	log_destroy(LOGGER);
}

