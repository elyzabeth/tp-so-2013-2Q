/*
 * planificador.c
 *
 *  Created on: Oct 8, 2013
 *      Author: elizabeth
 */

#include "plataforma.h"

t_personaje* moverPersonajeNuevoAListo(t_planificador *planner);
t_personaje* quitarPersonajeColaxId(t_queue *personajesListos, char idPersonaje);
t_personaje* moverPersonajeListoABloqueado(t_planificador *planner, char idPersonaje);
void planificarPersonaje(t_planificador *planner);
int enviarMsjTurnoConcedido(t_personaje *personaje, char* nivel);
int recibirCambiosConfiguracion(int32_t fdNivel, header_t header, t_planificador *planner);
int recibirSolicitudUbicacion(int fdPersonaje, header_t header, fd_set *master, t_planificador *planner );
int recibirUbicacionRecursoNivel( header_t header, fd_set *master, t_planificador *planner );

void* planificador(t_planificador *planner) {

	//t_personaje *personajeEjecutando = NULL;
	//t_queue *personajesListos = queue_create();
	//t_queue *personajesBloqueados = queue_create();
	t_personaje *personaje = NULL;
	header_t header;

	fd_set master;
	fd_set read_fds;
	int max_desc = 0;
	int i, ret;

	int se_desconecto;
	int fin = 0;
	struct timeval timeout;

	cambiarEstadoNivelaCorriendo(planner->nivel.nombre);
	log_info(LOGGER, "PLANIFICADOR %s: Iniciado.", planner->nivel.nombre);

	FD_ZERO(&master);

	// Agrego descriptor del Pipe con plataforma.
	agregar_descriptor(planner->fdPipe[0], &master, &max_desc);

	// Agrego descriptor del socket del nivel
	agregar_descriptor(planner->nivel.fdSocket, &master, &max_desc);

	while(!fin) {

		FD_ZERO (&read_fds);
		read_fds = master;
		timeout.tv_sec = planner->nivel.retardo * 0.001; /// retardo en Segundos timeout
		timeout.tv_usec = 0 ;

		ret = select(max_desc+1, &read_fds, NULL, NULL, &timeout);
		//ret = select(max_desc+1, &read_fds, NULL, NULL, NULL);
		//log_debug(LOGGER, "PLANIFICADOR %s: ret: %d ", planner->nivel.nombre, ret);
		// ret=-1 ERROR
		// ret= 0 timeout sin actividad en los fd
		if(ret == -1)
		{
			printf("PLANIFICADOR %s: ERROR en select.", planner->nivel.nombre);
			sleep(1);
			continue;
		}

		if (ret == 0) {
			//log_debug(LOGGER, "PLANIFICADOR %s: timeout", planner->nivel.nombre);
			// TODO Llamar a funcion que realiza la planificacion.
			planificarPersonaje(planner);
		}

		if (ret > 0) {

			for(i = 0; i <= max_desc; i++)
			{

				if (FD_ISSET(i, &read_fds))
				{
					if (i == planner->fdPipe[0])
					{
						header_t header;
						log_info(LOGGER, "PLANIFICADOR  %s: Recibo mensaje desde Plataforma por Pipe", planner->nivel.nombre);
						read (planner->fdPipe[0], &header, sizeof(header_t));

						log_debug(LOGGER, "PLANIFICADOR  %s: mensaje recibido '%d'", planner->nivel.nombre, header.tipo);
						if (header.tipo == NUEVO_PERSONAJE) {
							log_debug(LOGGER, "PLANIFICADOR  %s: mensaje recibido '%d' ES NUEVO_PERSONAJE", planner->nivel.nombre, header.tipo);
							personaje = moverPersonajeNuevoAListo(planner);
							agregar_descriptor(personaje->fd, &master, &max_desc);
						}
						if (header.tipo == FINALIZAR) {
							log_debug(LOGGER, "PLANIFICADOR  %s: '%d' ES FINALIZAR", planner->nivel.nombre, header.tipo);
							cambiarEstadoNivelaFinalizado(planner->nivel.nombre);
							fin = true;
							//FD_CLR(planner->fdPipe[0], &master);
							quitar_descriptor(planner->fdPipe[0], &master, &max_desc);
							break;
						}

					} else {
						// si NO es un mensaje de plataforma puede ser un nivel o un personaje

						log_info(LOGGER, "PLANIFICADOR %s: NUEVO MENSAJE en socket %d", planner->nivel.nombre, i);
						initHeader(&header);
						recibir_header(i, &header, &master, &se_desconecto);

						if(se_desconecto)
						{
							// Si se desconecto el Nivel Informo por pantalla y finalizo el hilo.
							if (i == planner->nivel.fdSocket) {
								log_info(LOGGER, "PLANIFICADOR %s: Se desconecto el Nivel (socket %d)", planner->nivel.nombre, i);
								cambiarEstadoNivelaFinalizado(planner->nivel.nombre);
								fin = true;
								quitar_descriptor(planner->fdPipe[0], &master, &max_desc);

								// TODO informar a quien corresponda la desconeccion del nivel.
								//informarDesconeccionPersonajes();
								break;
							} else {
								// TODO chequear si se desconecto personaje y borrarlo de las estructuras
								// si es personaje informar al nivel para que lo borre?
								log_debug(LOGGER, "PLANIFICADOR %s: socket %d recibo: %c", planner->nivel.nombre, i, header.tipo);

								continue;
							}
						}

						switch(header.tipo) {

							case CAMBIOS_CONFIGURACION: log_info(LOGGER, "PLANIFICADOR %s: CAMBIOS_CONFIGURACION", planner->nivel.nombre);
								recibirCambiosConfiguracion(i, header, planner);
								log_info(LOGGER, "\n\nNUEVOS VALORES\n**************\nAlgoritmo: %s\nQuantum: %d\nRetardo: %d\n", planner->nivel.algoritmo, planner->nivel.quantum, planner->nivel.retardo);
								break;

							case UBICACION_RECURSO: log_info(LOGGER, "PLANIFICADOR %s: UBICACION_RECURSO", planner->nivel.nombre);
								recibirUbicacionRecursoNivel( header, &master, planner);
								break;

							case SOLICITUD_UBICACION: log_info(LOGGER, "PLANIFICADOR %s: SOLICITUD_UBICACION", planner->nivel.nombre);
								recibirSolicitudUbicacion(i, header, &master, planner);
								break;

							case SOLICITUD_RECURSO: log_info(LOGGER, "PLANIFICADOR %s: SOLICITUD_RECURSO", planner->nivel.nombre);
								break;

							case MOVIMIENTO_REALIZADO: log_info(LOGGER, "PLANIFICADOR %s: MOVIMIENTO_REALIZADO", planner->nivel.nombre);
								//planner->personajeEjecutando->criterio--; Se resta cuando llega mensaje de movimiento realizado.
								break;

							case OTRO: log_info(LOGGER, "PLANIFICADOR %s: OTRO", planner->nivel.nombre);
								break;

							default: log_info(LOGGER, "PLANIFICADOR %s: MENSAJE NO RECONOCIDO '%d' ", planner->nivel.nombre, header.tipo);
						}
					}
				}
			}
		}
	}

	log_info(LOGGER, "FINALIZANDO HILO PLANIFICADOR '%s'", planner->nivel.nombre);

	// DEBO DESTRUIR LOS PERSONAJES?? NO SON COMPARTIDOS???? NO DEBERIA SOLO ELIMINAR LAS COLAS???
//	queue_destroy_and_destroy_elements(personajesListos, (void*)destruirPersonaje);
//	queue_destroy_and_destroy_elements(personajesBloqueados, (void*)destruirPersonaje);
	//queue_destroy(personajesListos);
	//queue_destroy(personajesBloqueados);

	moverPersonajesAFinAnormalxNivel(planner->nivel.nombre);

	pthread_exit(NULL);

}



t_personaje* moverPersonajeNuevoAListo(t_planificador *planner) {
	t_personaje* personaje;
	log_info(LOGGER, "moverPersonajeNuevoAListo Nivel '%s'", planner->nivel.nombre);

	personaje = quitarPersonajeNuevoxNivel(planner->nivel.nombre);
	agregarPersonajeEnJuego(personaje);

	log_info(LOGGER, "Agrego al personaje '%c' a la cola de listos del nivel '%s'.", personaje->id, planner->nivel.nombre);
	queue_push(planner->personajesListos, personaje);

	return personaje;
}


/**
 * @NAME: quitarPersonajeColaxId
 * @DESC: Quita un personaje de la cola de personajes buscandolo por id de personaje (simbolo)
 * Si lo encuentra devuelve el puntero a la estructura t_personaje.
 * Si no lo encuentra devuelve NULL.
 * Se le pasa la cola de personajes y el id del personaje buscado.
 */
t_personaje* quitarPersonajeColaxId(t_queue *colaPersonajes, char idPersonaje) {
	int tamanio = queue_size(colaPersonajes);
	int i;
	t_personaje *personaje = NULL;

	for (i = 0; i < tamanio; i++) {
		personaje = queue_pop(colaPersonajes);

		if(!personaje->id == idPersonaje){
			queue_push(colaPersonajes, personaje);
			personaje = NULL;
		}
	}

	return personaje;
}

t_personaje* moverPersonajeListoABloqueado( t_planificador *planner, char idPersonaje ) {
	t_personaje *personaje;
	log_info(LOGGER, "moverPersonajeListoABloqueado '%s': '%c'", planner->nivel.nombre, idPersonaje);

	personaje = quitarPersonajeColaxId(planner->personajesListos, idPersonaje);
	if (NULL != personaje) {
		queue_push(planner->personajesBloqueados, personaje);
		log_info(LOGGER, "Agrego al personaje '%c' a la cola de bloqueados del nivel '%s'.", personaje->id, planner->nivel.nombre);
	} else {
		log_warning(LOGGER, "WARNING! No se encontro el personaje '%c' en la cola de Listos del nivel '%s'!", idPersonaje, planner->nivel.nombre);
	}

	return personaje;
}

void planificarPersonaje(t_planificador *planner) {

	if(queue_size(planner->personajesListos)>0) {
		if (planner->personajeEjecutando == NULL || planner->personajeEjecutando->criterio == 0) {
			// TODO esto es RR falta agregar elegir personaje segun SRDF (apropiativo)
			planner->personajeEjecutando = queue_pop(planner->personajesListos);
			queue_push(planner->personajesListos, planner->personajeEjecutando);
			planner->personajeEjecutando->criterio = planner->nivel.quantum;
		}

		if (enviarMsjTurnoConcedido(planner->personajeEjecutando, planner->nivel.nombre)!=EXITO)
			log_error(LOGGER, "planificarPersonaje %s: ERROR al enviar Turno Concedido a '%s'", planner->nivel.nombre, planner->personajeEjecutando->nombre);
	}
}

int recibirCambiosConfiguracion(int fdNivel, header_t header, t_planificador *planner) {
	int ret;
	char *buffer;
	t_nivel nivel;

	buffer = calloc(1, header.largo_mensaje);
	if ((ret = recibir (fdNivel, buffer, header.largo_mensaje)) == EXITO) {
		initNivel(&nivel);
		memcpy(&nivel, buffer, sizeof(t_nivel));

		strcpy(planner->nivel.algoritmo, nivel.algoritmo);
		planner->nivel.quantum = nivel.quantum;
		planner->nivel.retardo = nivel.retardo;
	}

	free(buffer);

	return ret;
}

int enviarSolicitudUbicacionNivel ( header_t header, t_personaje personaje, t_planificador *planner ) {
	int ret;
	char* buffer;

	buffer = calloc(1, sizeof(header_t));
	memcpy(buffer, &header, sizeof(header_t));
	log_debug(LOGGER, "enviarSolicitudUbicacionNivel: Envio header (size: %d) SOLICITUD_UBICACION recurso %c al Nivel %s", sizeof(header_t), personaje.recurso, planner->nivel.nombre);

	if (enviar(planner->nivel.fdSocket, buffer, sizeof(header_t)) != EXITO)
	{
		log_error(LOGGER,"enviarSolicitudUbicacionNivel: Error al enviar header SOLICITUD_UBICACION\n\n");
		free(buffer);
		return WARNING;
	}

	free(buffer);
	buffer = calloc(1, sizeof(t_personaje));
	memcpy(buffer, &personaje, sizeof(t_personaje));

	log_debug(LOGGER, "enviarSolicitudUbicacionNivel: Envio t_personaje %d (%s, %c, %d, %d, %d, %s, recurso: %c)", sizeof(t_personaje), personaje.nombre, personaje.id, personaje.posActual.x, personaje.posActual.y, personaje.fd, personaje.nivel, personaje.recurso);
	if ((ret=enviar(planner->nivel.fdSocket, buffer, sizeof(t_personaje))) != EXITO)
	{
		log_error(LOGGER,"enviarSolicitudUbicacionNivel: Error al enviar informacion t_personaje en SOLICITUD_UBICACION\n\n");
		free(buffer);
		return ret;
	}

	free(buffer);

	return ret;
}


int recibirSolicitudUbicacion(int fdPersonaje, header_t header, fd_set *master, t_planificador *planner ) {
	int ret, se_desconecto;
	t_personaje personaje;

	log_debug(LOGGER, "Espero recibir estructura personaje (size:%d)...", header.largo_mensaje);
	ret = recibir_personaje(fdPersonaje, &personaje, master, &se_desconecto);
	log_debug(LOGGER, "Llego: %s (%c) al %s. Solicita recurso: '%c'", personaje.nombre, personaje.id, personaje.nivel, personaje.recurso);

	// TODO hacer algo con la info que llega!
	if (planner->personajeEjecutando->id == personaje.id)
		planner->personajeEjecutando->recurso = personaje.recurso;

	// Solicitar Ubicacion al NIVEL...
	ret = enviarSolicitudUbicacionNivel(header, personaje, planner);

	return ret;
}


int recibirUbicacionRecursoNivel( header_t header, fd_set *master, t_planificador *planner ) {
	int ret;
	t_caja caja;
	char *buffer;

	buffer = calloc(1, sizeof(t_caja));
	log_debug(LOGGER, "recibirUbicacionRecurso: Espero recibir estructura t_caja (size:%d)...", header.largo_mensaje);
	ret = recibir(planner->nivel.fdSocket, buffer, sizeof(t_caja));

	initCaja(&caja);
	memcpy(&caja, buffer, sizeof(t_caja));
	log_debug(LOGGER, "recibirUbicacionRecurso: Llego: %s (%c) posicion (%d, %d).", caja.RECURSO, caja.SIMBOLO, caja.POSX, caja.POSY);

	// TODO hacer algo con la info que llega!
	// Enviar Ubicacion al personaje en Juego...
	ret = enviar_header(planner->personajeEjecutando->fd, &header);
	ret = enviar_caja(planner->personajeEjecutando->fd, &caja);

	free(buffer);

	return ret;
}

int enviarMsjTurnoConcedido(t_personaje *personaje, char* nivel) {
	int ret;
	header_t header;
	char* buffer_header = malloc(sizeof(header_t));

	memset(&header, '\0', sizeof(header_t));
	header.tipo=TURNO_CONCEDIDO;
	header.largo_mensaje=0;

	memset(buffer_header, '\0', sizeof(header_t));
	memcpy(buffer_header, &header, sizeof(header_t));

	log_info(LOGGER, "%s: Envio mensaje de TURNO_CONCEDIDO a %s (fd: %d)...", nivel, personaje->nombre, personaje->fd);

	ret =  enviar(personaje->fd, buffer_header, sizeof(header_t));
	free(buffer_header);

	return ret;
}
