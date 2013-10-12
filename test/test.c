/*
 * test.c
 *
 *  Created on: Sep 23, 2013
 *      Author: elizabeth
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>

#include "commons/string.h"
#include "commons/collections/queue.h"
#include "tad_items.h"

t_list* GUIITEMS;
int MAXROWS, MAXCOLS;

void quitarCorchetes (char *to, char *from);
void GenerarListaObjetivos();
void getStringAsArray();

void nuevoHilo (void * param);
void testHilo();
void testNivelGui();



int main () {

	//GenerarListaObjetivos();
	//getStringAsArray();
	//testHilo();
	testNivelGui();

	return 0;
}

void testNivelGui(){
	pthread_t idHilo;

	GUIITEMS = list_create();

	nivel_gui_inicializar();
    nivel_gui_get_area_nivel(&MAXROWS, &MAXCOLS);
    nivel_gui_dibujar(GUIITEMS, "TEST");

	pthread_create(&idHilo, NULL, (void*)nuevoHilo, NULL);
	while (1) {
		//puts("testHilo: sigo por aca");
		 nivel_gui_dibujar(GUIITEMS, "TEST");
		sleep(1);
	}
}


void nuevoHilo (void * param) {
	//puts("nuevoHilo: Comienza");

	while (1) {
		//puts("nuevoHilo: en el while");
		nivel_gui_dibujar(GUIITEMS, "TEST");
		sleep(5);
	}

	pthread_exit(NULL);
}

void testHilo(){
	pthread_t idHilo;

	pthread_create(&idHilo, NULL, (void*)nuevoHilo, NULL);
	while (1) {
		//puts("testHilo: sigo por aca");
		sleep(1);
	}


}


void getStringAsArray() {
	char *planDeNiveles="[Nivel3,Nivel4,Nivel1]";
	char **plan = string_get_string_as_array(planDeNiveles);

	void _imprimir(char *a){
		printf("planDeNiveles %s\n", a);
	}

	string_iterate_lines(plan, _imprimir);
	string_iterate_lines(plan, (void*) free);
	free(plan);

}

void testQuitarCorchetes () {
	char *planDeNiveles="[[Nivel3,[Nivel4,Nivel1]";
	char plan[100] = {0};
	quitarCorchetes(plan, planDeNiveles);

	printf("plan: %s", plan);
}

void quitarCorchetes (char *to, char *from) {
	int i = 0, j = 0;

	while ( i < strlen(from) ){
		if(from[i] != ']' && from[i] != '[')
			to[j++] = from[i++];
		else
			i++;
	}

	to[j] = '\0';

}

void GenerarListaObjetivos() {
	// TODO Armar la lista FIFO dinamica del Plan de niveles y los objetivos.
	char plan[200+1]={0};
	typedef struct {
		char nivel[200+1];
		char objetivos[50][2];
	} t_objetivosxNivel;

	char objetivo[200+1]={0};
	char key[20] = { 0 };
	char** substring;
	char** recursos;
	int i=0;
	t_queue *PLANDENIVELES = queue_create();

	// planDeNiveles=[Nivel3,Nivel4,Nivel1]
	// obj[Nivel1]=[F,H,F,M]
	// obj[Nivel3]=[C,J,C]
	// obj[Nivel4]=[P,Q,M]

	// Quito los corchetes de la expresion "[Nivel3,Nivel4,Nivel1]"
	quitarCorchetes(plan, "[Nivel3,Nivel4,Nivel1]");
	//quitarCorchetes(plan, config_get_string_value(config, "PLANDENIVELES"));
	substring = string_split(plan, ",");

	void _add_objetives(char *nivel) {
		t_objetivosxNivel *objxniv;
		objxniv = (t_objetivosxNivel*)malloc(sizeof(t_objetivosxNivel));
		strcpy(objxniv->nivel, nivel);
		sprintf(key, "obj[%s]", nivel );

		// Quito los corchetes de la expresion "[F,H,F,M]"
		quitarCorchetes(objetivo, "[F,H,F,M]");
		//quitarCorchetes(objetivo, config_get_string_value(config, key));
		recursos = string_split(objetivo, ",");

		void _add_resource(char *rec) {
			objxniv->objetivos[i][0] = rec[0];
			objxniv->objetivos[i][1] = 0;
			i++;
		}

		string_iterate_lines(recursos, _add_resource);
		queue_push(PLANDENIVELES, objxniv);

		string_iterate_lines(recursos, (void*)free);
		free(recursos);

	}

	string_iterate_lines(substring, _add_objetives);
	string_iterate_lines(substring, (void*) free);
	free(substring);

	t_objetivosxNivel *oxn;

	while(!queue_is_empty(PLANDENIVELES)) {
		oxn = (t_objetivosxNivel*)queue_pop(PLANDENIVELES);
		printf(" nivel: %s \n", oxn->nivel);
		free(oxn);
	}

	queue_destroy_and_destroy_elements(PLANDENIVELES, (void*)free);

}


