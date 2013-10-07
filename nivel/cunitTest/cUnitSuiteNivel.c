/*
 * cUnitSuiteNivel.c
 *
 *  Created on: 23/09/2013
 *      Author: elyzabeth
 */

#include "cUnitSuiteNivel.h"

void testNivel01 () {
	printf("Soy testNivel01 y pruebo que xxxxxxxxx sea igual a xxxxxxxxxx: ");
	inicializarNivel();
	CU_ASSERT_STRING_EQUAL(configNivelNombre(), "nivel1");
}

int suiteNivel () {
	CU_initialize_registry();

	CU_pSuite prueba = CU_add_suite("Suite de prueba Nivel ", NULL, NULL);
	CU_add_test(prueba, "1", testNivel01);
//	CU_add_test(prueba, "2", testNivel2);
//	CU_add_test(prueba, "3", testNivel3);
//	CU_add_test(prueba, "4", testNivel4);
//	CU_add_test(prueba, "5", testNivel5);
//	CU_add_test(prueba, "6", testNivel6);
//	CU_add_test(prueba, "7", testNivel7);
//	CU_add_test(prueba, "8", testNivel8);
//	CU_add_test(prueba, "9", testNivel9);
//	CU_add_test(prueba, "10", testNivel10);
//	CU_add_test(prueba, "11", testNivel11);
//	CU_add_test(prueba, "12", testNivel12);
//	CU_add_test(prueba, "13", testNivel13);
//	CU_add_test(prueba, "14", testNivel14);
	//TODO falta agregar caso configRecurso y configRecursos

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	finalizarNivel();

	return CU_get_error();
}
