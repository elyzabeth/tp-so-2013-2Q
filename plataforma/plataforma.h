
#ifndef plataforma_H_
#define plataforma_H_

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <pthread.h>

#include "commons/comunicacion.h"
#include "config/configPlataforma.h"
#include "commons/log.h"
#include <commons/collections/list.h>
#include "tads/tad_personaje.h"

typedef struct config_plataforma_s
{
  //char id[LARGOID];
  char nombre [10];
  char cantidad_conexiones; /*igual a cantidad de personajes?????*/
  char personajes_en_juego;

}config_plataforma_t;

t_log* LOGGER;
int32_t PUERTO;
config_plataforma_t plataforma;

void inicializarPlataforma();
void finalizarPlataforma();

void* orquestador(void *parametro);
void nuevoPersonaje(int fdPersonaje);

void plat_signal_callback_handler(int signum);

#endif /* plataforma_H_ */



