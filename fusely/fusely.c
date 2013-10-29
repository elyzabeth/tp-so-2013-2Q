/*
 * fusely.c
 *
 *  Created on: 09/10/2013
 *      Author: elyzabeth
 */

#include <stdio.h>
#include <stdlib.h>

#include <stddef.h>

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>

#define GFILEBYTABLE 1024
#define GFILEBYBLOCK 1
#define GFILENAMELENGTH 71
#define GHEADERBLOCKS 1
#define BLKINDIRECT 1000
#define BLOCK_SIZE 4096 //BYTES


typedef u_int8_t               uint8_t;
typedef u_int16_t              uint16_t;
typedef u_int32_t              uint32_t;
typedef u_int64_t              uint64_t;

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
	unsigned char fname[GFILENAMELENGTH];
	uint32_t parent_dir_block;
	uint32_t file_size;
	uint64_t c_date;
	uint64_t m_date;
	ptrGBloque blk_indirect[BLKINDIRECT];
} GFile;

GHeader HEADER;
GFile ARCHIVO;
char *BITMAP;
GFile TABLANODOS[GFILEBYTABLE];

void leerArchivo1() {
	FILE* archivoBin = NULL;
	FILE* archivoTXT = NULL;

	void* BUF[4096] = {0};
	//int tamanio = 5130;
	int bloq[2] = {1357, 1310};
	int i ;

	GHeader header;
	//GFile archivo;

	archivoBin = fopen("disk.bin", "rb");
	archivoTXT = fopen("secreto.txt", "w+");
	fread(&header, sizeof(GHeader), 1, archivoBin);
	//fread(&rArchivoNiveles, sizeof(rArchivoNiveles), 1,	archivoNiveles);

	for (i=0; i < 2-1; i++) {
		fseek(archivoBin, 4096*bloq[i], SEEK_SET);
		fread(&BUF, 4096, 1, archivoBin);
		//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
		fwrite(&BUF, 4096, 1, archivoTXT);
	}

	fseek(archivoBin, 4096*bloq[i], SEEK_SET);
	fread(&BUF, 1034, 1, archivoBin);
	//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
	fwrite(&BUF, 1034, 1, archivoTXT);

	fclose(archivoBin);
	fclose(archivoTXT);

}

void leerArchivo2() {
	FILE* archivoBin = NULL;
	FILE* archivoTXT = NULL;
	//void BUFFER[10485760];
	//int tamanio = 280722; // 280722 - 278528 = 2194
	void* BUF[4096] = {0};
	int bloq[69] = {1341, 1458, 2419, 2463, 2159, 2330, 1837, 2095, 1268, 1882, 2209, 1138, 1805, 2279, 1409, 1545, 2039, 1876, 1656, 1250, 2020, 1888, 1756, 2035, 2522, 1874, 1497, 1580, 1448, 2121, 2275, 2173, 1419, 1931, 1976, 2377, 1493, 2131, 1112, 2341, 1787, 2211, 1749, 2291, 1316, 2427, 2461, 2220, 2527, 1517, 1748, 1985, 2267, 1648, 1984, 1997, 2540, 2167, 2003, 1381, 1803, 1194, 2524, 1460, 1439, 1808, 2170, 1600, 1617};
	int i;
	GHeader header;
	//GFile archivo;

	archivoBin = fopen("disk.bin", "rb");
	archivoTXT = fopen("frase.mp4", "w+");
	fread(&header, sizeof(GHeader), 1, archivoBin);
	//fread(&rArchivoNiveles, sizeof(rArchivoNiveles), 1,	archivoNiveles);

	for (i=0; i < 69-1; i++) {
		fseek(archivoBin, 4096*bloq[i], SEEK_SET);
		fread(&BUF, 4096, 1, archivoBin);
		//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
		fwrite(&BUF, 4096, 1, archivoTXT);
	}
	fseek(archivoBin, 4096*bloq[i], SEEK_SET);
	fread(&BUF, 2194, 1, archivoBin);
	//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
	fwrite(&BUF, 2194, 1, archivoTXT);

	fclose(archivoBin);
	fclose(archivoTXT);

}


void leerArchivo3() {
	FILE* archivoBin = NULL;
	FILE* archivoTXT = NULL;
	//void BUFFER[10485760];

	//int tamanio = 156032; // 156032 - 155648 = 384
	void* BUF[4096] = {0};
	int bloq[39] = {2258, 1136, 1379, 1438, 2151, 1387, 1610, 2350, 2083, 1917, 2557, 1388, 2487, 2290, 2276, 1414, 2468, 1113, 2099, 2255, 2248, 1982, 1794, 1461, 1524, 2498, 1745, 2390, 1638, 1677, 1432, 1059, 2516, 1224, 2086, 1869, 2117, 1157, 1860};
	int i;
	GHeader header;
	//GFile archivo;

	archivoBin = fopen("disk.bin", "rb");
	archivoTXT = fopen("top_secret.jpg", "w+");
	fread(&header, sizeof(GHeader), 1, archivoBin);
	//fread(&rArchivoNiveles, sizeof(rArchivoNiveles), 1,	archivoNiveles);

for (i=0; i < 39-1; i++) {
	fseek(archivoBin, 4096*bloq[i], SEEK_SET);
	fread(&BUF, 4096, 1, archivoBin);
	//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
	fwrite(&BUF, 4096, 1, archivoTXT);
}

fseek(archivoBin, 4096*bloq[i], SEEK_SET);
fread(&BUF, 384, 1, archivoBin);
//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
fwrite(&BUF, 384, 1, archivoTXT);

	fclose(archivoBin);
	fclose(archivoTXT);

}


void leerArchivo4() {
	FILE* archivoBin = NULL;
	FILE* archivoTXT = NULL;
	//void BUFFER[10485760];
	//int tamanio = 13; // 280722 - 278528 = 2194
	void* BUF[4096] = {0};
	//int bloq[1] = {2344};
	//int i;
	GHeader header;
	//GFile archivo;

	archivoBin = fopen("disk.bin", "rb");
	archivoTXT = fopen("hola.txt", "w+");
	fread(&header, sizeof(GHeader), 1, archivoBin);
	//fread(&rArchivoNiveles, sizeof(rArchivoNiveles), 1,	archivoNiveles);

fseek(archivoBin, 4096*2344, SEEK_SET);
fread(&BUF, 13, 1, archivoBin);
//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
fwrite(&BUF, 13, 1, archivoTXT);

	fclose(archivoBin);
	fclose(archivoTXT);

}

void leerArchivo5() {
	FILE* archivoBin = NULL;
	FILE* archivoTXT = NULL;
	//void BUFFER[10485760];
	//int tamanio = 7; // 280722 - 278528 = 2194
	void* BUF[4096] = {0};
	//int i;
	GHeader header;
	//GFile archivo;

	archivoBin = fopen("disk.bin", "rb");
	archivoTXT = fopen("archivo", "w+");
	fread(&header, sizeof(GHeader), 1, archivoBin);
	//fread(&rArchivoNiveles, sizeof(rArchivoNiveles), 1,	archivoNiveles);


fseek(archivoBin, 4096*1540, SEEK_SET);
fread(&BUF, 7, 1, archivoBin);
//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
fwrite(&BUF, 7, 1, archivoTXT);

	fclose(archivoBin);
	fclose(archivoTXT);

}


void leerArchivo6() {
	FILE* archivoBin = NULL;
	FILE* archivoTXT = NULL;
	//void BUFFER[10485760];
	//int tamanio = 46784; // 46784 - 45056 = 1728
	void* BUF[4096] = {0};
	int bloq[12] = {2450, 1978, 2169, 1099, 1542, 1395, 1361, 2201, 1121, 2136, 1729, 1229};
	int i;
	GHeader header;
	//GFile archivo;

	archivoBin = fopen("disk.bin", "rb");
	archivoTXT = fopen("ar", "w+");
	fread(&header, sizeof(GHeader), 1, archivoBin);
	//fread(&rArchivoNiveles, sizeof(rArchivoNiveles), 1,	archivoNiveles);

	for (i=0; i < 12-1; i++) {
		fseek(archivoBin, 4096*bloq[i], SEEK_SET);
		fread(&BUF, 4096, 1, archivoBin);
		//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
		fwrite(&BUF, 4096, 1, archivoTXT);
	}
	fseek(archivoBin, 4096*bloq[i], SEEK_SET);
	fread(&BUF, 1728, 1, archivoBin);
	//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
	fwrite(&BUF, 1728, 1, archivoTXT);

	fclose(archivoBin);
	fclose(archivoTXT);

}

void leerArchivo7() {
	FILE* archivoBin = NULL;
	FILE* archivoTXT = NULL;
	//int tamanio = 108612;
	void* BUF[4096] = {0};
	int bloq[27] = {1577, 2353, 1507, 2324, 1233, 1115, 1914, 1441, 2559, 2366, 1087, 1477, 1676, 2431, 1100, 2473, 2339, 2089, 1465, 1201, 2218, 1042, 1392, 1646, 1858, 2402, 1330};
	int i;
	GHeader header;
	//GFile archivo;

	archivoBin = fopen("disk.bin", "rb");
	archivoTXT = fopen("chivo", "w+");
	fread(&header, sizeof(GHeader), 1, archivoBin);
	//fread(&rArchivoNiveles, sizeof(rArchivoNiveles), 1,	archivoNiveles);

	for (i=0; i < 27-1; i++) {
		fseek(archivoBin, 4096*bloq[i], SEEK_SET);
		fread(&BUF, 4096, 1, archivoBin);
		//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
		fwrite(&BUF, 4096, 1, archivoTXT);
	}
	fseek(archivoBin, 4096*bloq[i], SEEK_SET);
	fread(&BUF, 2116, 1, archivoBin);
	//fwrite(&rArchivoPersonajes, sizeof(rArchivoPersonajes), 1, archivoPersonajes);
	fwrite(&BUF, 2116, 1, archivoTXT);

	fclose(archivoBin);
	fclose(archivoTXT);

}

void leerArchivos(){
	leerArchivo1();
	leerArchivo2();
	leerArchivo3();
	leerArchivo4();
	leerArchivo5();
	leerArchivo6();
	leerArchivo7();
}

void levantarHeader(FILE* archivoBin) {
	// Esta en el bloque 0
	fread(&HEADER, sizeof(GHeader), 1, archivoBin);
}

void imprimirHeader() {
	printf("Nombre: %s\nVersion: %u\nbloque Bitmap: %u\nTamaño Bitmap: %u\n", HEADER.grasa, HEADER.version, HEADER.blk_bitmap, HEADER.size_bitmap);
}

void levantarBitmap(FILE* archivoBin) {
	// Esta en el bloque 1
	BITMAP = malloc(BLOCK_SIZE);
	fseek(archivoBin, BLOCK_SIZE*HEADER.blk_bitmap, SEEK_SET);
	fread(&BITMAP, BLOCK_SIZE, 1, archivoBin);
}

void liberarBitmap() {
	free(BITMAP);
}

void levantarTablaNodos(FILE* archivoBin) {
	fseek(archivoBin, BLOCK_SIZE*(1+HEADER.blk_bitmap), SEEK_SET);
	fread(&TABLANODOS, BLOCK_SIZE*GFILEBYTABLE, 1, archivoBin);
}

void imprimirNodo (GFile nodo) {
	int i;
	printf("Estado: %u\nNombre: %s\nBloque Padre: %u\nTamaño: %u\n", nodo.state, nodo.fname, nodo.parent_dir_block, nodo.file_size);
	printf("Fecha Creacion: %" PRIu64 "\nFecha Modificacion: %" PRIu64 "\nBloques: \n", nodo.c_date, nodo.m_date);

	if (nodo.file_size > 0) {
		for(i = 0; i < (nodo.file_size/BLOCK_SIZE)+1; i++){
			printf(" %d) %u\n", i, nodo.blk_indirect[i]);
		}
	}
}


int main () {
	FILE* archivoBin = NULL;
	archivoBin = fopen("disk.bin", "rb");

	levantarHeader(archivoBin);
	imprimirHeader();

	levantarBitmap(archivoBin);

	levantarTablaNodos(archivoBin);
	imprimirNodo(TABLANODOS[0]);

	liberarBitmap();
	fclose(archivoBin);

	return 1;
}
