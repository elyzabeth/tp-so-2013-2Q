/*
 * fileSystem.h
 *
 *  Created on: 18/10/2013
 *      Author: arwen
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <time.h>

// ELIMINAR ESTAS 3 LINEAS!!
#define DEFAULT_FILE_CONTENT "Hello World!\n"
#define DEFAULT_FILE_NAME "hello"
#define DEFAULT_FILE_PATH "/" DEFAULT_FILE_NAME



#define GFILEBYTABLE 1024
#define GFILEBYBLOCK 1
#define GFILENAMELENGTH 71
#define GHEADERBLOCKS 1
#define BLKINDIRECT 1000
#define BLKLEN 4096 //Tamaño de bloque fijo en bytes
#define TAMANIO 10485760

typedef uint32_t ptrGBloque;

typedef struct grasa_header_t { // un bloque
	unsigned char grasa[5];
	uint32_t version;
	uint32_t blk_bitmap;
	uint32_t size_bitmap; // en bloques
	unsigned char padding[4073];
} GHeader;

typedef struct grasa_file_t { // un cuarto de bloque (256 bytes)
	uint8_t state; // 0: borrado, 1: archivo, 2: directorio
	//unsigned char fname[GFILENAMELENGTH];
	char fname[GFILENAMELENGTH];
	uint32_t parent_dir_block;
	uint32_t file_size;
	uint64_t c_date;
	uint64_t m_date;
	ptrGBloque blk_indirect[BLKINDIRECT];
} GFile;

GHeader HEADER;
GFile ARCHIVO;

char *BITMAP;
void *TABLANODOS;
GFile *FNodo;
GFile *NODOS[GFILEBYTABLE]; // un array de 1024 posiciones de estructuras de tipo GFile

char *DATOS;

#endif /* FILESYSTEM_H_ */
