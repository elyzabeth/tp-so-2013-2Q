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
	int32_t id = (int32_t) idEnemigo;
	int32_t sleepEnemigos = configNivelSleepEnemigos();

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

		usleep(sleepEnemigos);
	}

	pthread_exit(NULL);
}
