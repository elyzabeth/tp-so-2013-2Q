/*
 * enemigo.c
 *
 *  Created on: Oct 11, 2013
 *      Author: elizabeth
 */

#include "funcionesNivel.h"


ITEM_NIVEL* _search_enemy_by_id(t_list* items, char id) {
	bool _search_by_id(ITEM_NIVEL* item) {
		return item->id == id;
	}

	return list_find(items, (void*) _search_by_id);
}

void* enemigo(int32_t *idEnemigo) {
	int x=10, y=15;
	char buffer[4] = {0};
	fd_set master;
	fd_set read_fds;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	int max_desc = 0;

	int32_t id = (int32_t) idEnemigo;
	// Obtengo parametros del archivo de configuracion
	int32_t sleepEnemigos = configNivelSleepEnemigos();

	agregar_descriptor(fdPipeMainToEnemy[0], &master, &max_desc);

	rnd(&x, MAXCOLS);
	rnd(&y, MAXROWS);
	log_info(LOGGER, "Creo Enemigo '%c'", id);
	//CrearEnemigo(GUIITEMS, id, x, y);
	nivel_gui_dibujar(GUIITEMS, NOMBRENIVEL);

	while (1) {
		//TODO agregar logica del enemigo
		// Cambiar ese movimiento aleatorio por el que corresponde
		rnd(&x, MAXCOLS);
		rnd(&y, MAXROWS);

		MoverPersonaje(GUIITEMS, id, x, y );
		//nivel_gui_dibujar(GUIITEMS, NOMBRENIVEL);

		read (fdPipeMainToEnemy[0], buffer, 4);

		usleep(sleepEnemigos);
	}

	pthread_exit(NULL);
}
