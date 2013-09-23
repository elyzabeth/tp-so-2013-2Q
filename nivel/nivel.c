/*
 * nivel.c
 *
 *  Created on: 22/09/2013
 *      Author: elyzabeth
 */

#include "nivel.h"


int main (int argc, char**argv) {

	// Correr tests
	if (strcmp(argv[1], "-test")==0)
		return correrTest();

	inicializar ();
	principal ();
	finalizar ();

	return EXIT_SUCCESS;
}

int correrTest() {
	return suiteNivel();
}

void inicializar () {
	levantarArchivoConfiguracionNivel();
	LOGGER = log_create(configNivelLogPath(), "NIVEL", configNivelLogConsola(), configNivelLogNivel() );
	strncpy(NOMBRENIVEL, configNivelNombre(), 20);
	log_info(LOGGER, "INICIALIZANDO NIVEL '%s' ", NOMBRENIVEL);

}

void finalizar () {
	log_info(LOGGER, "FINALIZANDO NIVEL '%s'", NOMBRENIVEL);
	destruirConfigNivel();
	log_destroy(LOGGER);
}

void principal () {

}

