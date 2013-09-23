/*
 * paquetes.h
 *
 *  Created on: Aug 9, 2013
 *      Author: elizabeth
 */

#ifndef PAQUETES_H_
#define PAQUETES_H_

#include <stdlib.h>

typedef enum {
	HANDSHAKE=0,
    HANDSHAKE_OK=1,
    HANDSHAKE_ERROR=2
} t_mensaje;

typedef struct {
	int16_t tipo;
	int16_t length;
} t_header;


#endif /* PAQUETES_H_ */
