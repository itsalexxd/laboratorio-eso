#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main (int argc, char *argv[]) {
	// Comprobamos que se ha introducido bien el comando
	if (argc < 2) {
		printf("UVazip: file1 [file2 ...]\n");
		exit(1);
	}	

	// Variables que vamos a necesitar
	int32_t cont = 0;	// Cuenta cuantos caracteres iguales llevamos
	int c;			// Almacenara el char que se lee del archivo
	int aux; 		// Almacen del char para comprobar los cambios
				
	// Bucle que pasa por todos los ficheros que se pasen como argc
	int indice = 1;
	while ( indice < argc) {
		// Abrimos los archivos
		FILE *fichero = fopen(argv[indice], "r");
		// Comprobamos que no esta vacio
		if (fichero == NULL) {
			printf("UVazip: cannot open file\n");
			exit(1);
		}
		
		// Leemos char por char del archivo
		while ((c = fgetc(fichero)) != EOF) {
			// Almaceno el char en aux para comprobar cuando cambiamos de char
			if (cont == 0) {
				aux = c;
			}

			if (aux != c) {
				// Escribimos por stdout el resumen
				fwrite(&cont, sizeof(int32_t), 1, stdout);
				fwrite(&aux, sizeof(char), 1, stdout);
				// Almacenamos el nuevo char y reiniciamos el cont
				aux = c;
				cont = 1;	// Pq ya hemos contado 1 char de los nuevos
			}else {
				cont++;
			}
		}
	

		// Cerramos el archivo 
		fclose(fichero);
		
		// Fin del archivo, pasamos al siguiente si hay
		indice++;
	}

	if (cont > 0) {
		fwrite(&cont, sizeof(int32_t), 1, stdout);
		fwrite(&aux, sizeof(char), 1 ,stdout);
	}
	// Programa exitoso
	return 0;
}
