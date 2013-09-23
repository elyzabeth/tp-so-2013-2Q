/*
 * configPersonaje.c
 *
 *  Created on: 22/09/2013
 *      Author: arwen
 */

#include "configPersonaje.h"



typedef struct {
	char NOMBRE[MAXCHARLEN+1];			// nombre=Mario
	char SIMBOLO;						// simbolo=@
	char PLANDENIVELES[MAXCHARLEN+1];	// planDeNiveles=[Nivel3,Nivel4,Nivel1]
	int32_t VIDAS;						// vidas=5
	char PLATAFORMA[MAXCHARLEN+1];		// orquestador=192.168.0.100:5000
	char PLATAFORMAIP[MAXCHARLEN+1];	//
	int32_t PLATAFORMAPUERTO;			//
	char LOG_PATH[MAXCHARLEN+1];		//ej LOG_PATH=/tmp/plataforma.log
	t_log_level LOG_NIVEL;				//ej LOG_NIVEL=TRACE|DEBUG|INFO|WARNING|ERROR
	int32_t LOG_PANTALLA;				// LOG_PANTALLA=0|1 (off/on)
} t_configPersonaje;

t_configPersonaje configPersonaje;

void inicializarconfigPersonaje () {
	configPersonaje.VIDAS = 0;
	configPersonaje.PLATAFORMAPUERTO = 0;
	configPersonaje.NOMBRE[0]='\0';
	configPersonaje.PLATAFORMA[0]='\0';
	configPersonaje.PLATAFORMAIP[0]='\0';
	configPersonaje.SIMBOLO=0;
	configPersonaje.LOG_PATH[0]='\0';
	configPersonaje.LOG_NIVEL=0;
	configPersonaje.LOG_PANTALLA=0;
	//configPersonaje.RECURSOS = dictionary_create();
}


/// FUNCIONES PRIVADAS
// *********************
//void separarIpPuertoPersonaje (char *ipPuerto) {
//	char** substring;
//	substring = (char**)string_split(ipPuerto, ":");
//	strcpy(configPersonaje.PLATAFORMAIP, substring[0]);
//	configPersonaje.PLATAFORMAPUERTO = atoi(substring[1]);
//
//	string_iterate_lines(substring, (void*) free);
//	free(substring);
//}

void GenerarListaObjetivos(t_config *config) {

}

void levantarArchivoConfiguracionPersonaje () {
	t_config *config;
	config = config_create(PATH_CONFIG_PERSONAJE);

	if (config->properties->elements_amount == 0) {
		printf("\nERROR AL LEVANTAR ARCHIVO DE CONFIGURACION %s ", PATH_CONFIG_PERSONAJE);
		perror("\nERROR AL LEVANTAR ARCHIVO DE CONFIGURACION");
		config_destroy(config);
		exit(-1);
	}

	//Inicializo Estructura
	inicializarconfigPersonaje(configPersonaje);

	//Levanto los parametros necesarios para el Personaje
	strcpy(configPersonaje.NOMBRE, config_get_string_value(config, "nombre"));
	configPersonaje.SIMBOLO = config_get_int_value(config, "simbolo");

	strcpy(configPersonaje.PLATAFORMA, config_get_string_value(config, "orquestador"));
	//SPLIT DE PLATAFORMA PARA SEPARAR IP DE PUERTO
	separarIpPuerto(configPersonaje.PLATAFORMA, configPersonaje.PLATAFORMAIP, &(configPersonaje.PLATAFORMAPUERTO));

	configPersonaje.VIDAS = config_get_int_value(config, "vidas");

	strcpy(configPersonaje.PLANDENIVELES, config_get_string_value(config, "planDeNiveles"));

	strcpy(configPersonaje.LOG_PATH, config_get_string_value(config, "LOG_PATH"));
	configPersonaje.LOG_NIVEL = obtenerLogLevel(config_get_string_value(config, "LOG_NIVEL"));
	configPersonaje.LOG_PANTALLA = config_get_int_value(config, "LOG_PANTALLA");

	// Armo lista dinamica de recursos
	GenerarListaObjetivos (config);

	// Una vez que se levantaron los datos del archivo de configuracion
	// puedo/debo destruir la estructura config.
	config_destroy(config);
}
