#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
	// Comprobamos que se ha ejecutado bien el programa
	if (argc < 2) {
		printf("UVaunzip: file1 [file2 ...]\n");
		exit(1);
	}
	
	// Variables que vamos a usar
	int cant = 0;
	char c;

	// Bucle para descomprimir
	int indice = 1;
	while (indice < argc) {
		// Abrirmos los ficheros
		FILE *archivo = fopen(argv[indice], "r");
		
		// Verificamos que el archivo se puede leer
		if (archivo == NULL) {
			printf("UVaunzip: cannot open file\n");
			exit(1);	
		}

		// Leemos el archivo abierto
		while (fread(&cant, sizeof(int), 1, archivo) == 1) {
			fread(&c, sizeof(char), 1, archivo);

			for (int i = 0; i < cant; i++) {
				printf("%c", c);
			}
		}

		// Cerramos el archivo y pasamos al siguiente 
		fclose(archivo);
		indice++;
	}
	
	
	// Programa exitoso
	return 0;
}
