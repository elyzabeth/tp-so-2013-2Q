/*
 * nivel.h
 *
 *  Created on: 22/09/2013
 *      Author: elyzabeth
 */

#ifndef NIVEL_H_
#define NIVEL_H_

#include <stdio.h>
#include <stdlib.h>

#include "commons/log.h"
#include "cunitTest/cUnitSuiteNivel.h"
#include "config/configNivel.h"

t_log* LOGGER;
char NOMBRENIVEL[20+1];

int correrTest();
void inicializar ();
void finalizar ();

#endif /* NIVEL_H_ */
