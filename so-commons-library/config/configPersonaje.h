/*
 * configPersonaje.h
 *
 *  Created on: 22/09/2013
 *      Author: arwen
 */

#ifndef CONFIGPERSONAJE_H_
#define CONFIGPERSONAJE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../commons/config.h"
#include "../commons/string.h"
#include "../commons/log.h"
#include "funciones.h"

#define PATH_CONFIG_PERSONAJE "./personaje.conf"
#define MAXCHARLEN 200

void levantarArchivoConfiguracionPersonaje ();

#endif /* CONFIGPERSONAJE_H_ */
