/*
 * personaje.h
 *
 *  Created on: Sep 24, 2013
 *      Author: elizabeth
 */

#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#include "config/configPersonaje.h"
#include "commons/log.h"
#include "commons/comunicacion.h"
#include "commons/funciones_comunes.h"
#include "tads/tad_personaje.h"

t_log* LOGGER;
int32_t VIDAS;
int32_t REINTENTOS;

char *buffer_header;

t_queue *planDeNiveles;
t_list *listaHilosxNivel;

typedef enum motivo_muerte {
	MUERTE_POR_SIGTERM,
	MUERTE_POR_SIGKILL,
	MUERTE_POR_ENEMIGO,
	MUERTE_POR_INTERBLOQUEO
} MOTIVO_MUERTE;

typedef struct personaje_s
{
	//char id[LARGOID];
	char nombre [25];
	char ip [15+1];
	int puerto;
	char ip_orquestador[15+1];
	int puerto_orquestador;
} personaje_t;


typedef struct hilo {
	t_personaje personaje;
	t_objetivosxNivel objetivos;
	int32_t tid;
	int32_t fdPipe[2];
} t_hilo_personaje;

personaje_t personaje;

int principal(int argc, char *argv[]);
void inicializarPersonaje();
void finalizarPersonaje();

void levantarHilosxNivel() ;
void* personajexNivel (t_hilo_personaje *hiloPxN);
t_hilo_personaje* crearHiloPersonaje();
void destruirHiloPersonaje(t_hilo_personaje* hiloPersonaje);

void per_signal_callback_handler(int signum);
void manejoSIGTERM();

int32_t incrementarVida();
int32_t decrementarVida();

int recibirHeaderNuevoMsj (int sock, header_t *header) ;
int enviarMsjNuevoPersonaje( int sock );
int enviarInfoPersonaje(int sock);

#endif /* PERSONAJE_H_ */
