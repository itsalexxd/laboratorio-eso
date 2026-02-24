#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

	while(1){

		printf("UVash> ");
		
		// Espero a que el usuario inserte un comando
		char *linea = NULL;
		size_t capacidad = 0;
		ssize_t leidos;

		leidos = getline(&linea, &capacidad, stdin);

		//printf("%ld", leidos);
		fputs(linea, stdout);
		printf("\n\n");

	}

}
