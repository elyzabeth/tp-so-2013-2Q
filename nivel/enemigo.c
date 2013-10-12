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

void* enemigo(void *idEnemigo) {
	int x=211, y=135;
	char id = ((char*)(idEnemigo))[0];
	int32_t sleepEnemigos = configNivelSleepEnemigos();

//	ITEM_NIVEL* item = _search_enemy_by_id(GUIITEMS, id);
//	int x = item->posx;
//	int y = item->posy;

	rnd(&x, MAXCOLS);
	rnd(&y, MAXROWS);
	CrearEnemigo(GUIITEMS, id, x, y);

	while (1) {
		//TODO agregar logica del enemigo
		// Cambiar ese movimiento aleatorio por el que corresponde
		rnd(&x, MAXCOLS);
		rnd(&y, MAXROWS);

		MoverPersonaje(GUIITEMS, id, x, y );
		nivel_gui_dibujar(GUIITEMS, NOMBRENIVEL);

		usleep(sleepEnemigos);
	}

	pthread_exit(NULL);
}
