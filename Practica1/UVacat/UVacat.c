#include <stdio.h>
#include <stdlib.h>

int main(int argc, char*argv[]) {
	// Comprobamos que la ejecucion del programa es valida
	if (argc <  2) {
		//printf ("EROR\n");
		//printf("Uso: ./UVacat <archivo1> [archivo2] [archivo3] ... \n");
		return (0);
	}
	int cont = 1;
	char buffer[255];
	while (cont < argc) {		
		// Abrimos el archivo
		FILE *fichero = fopen(argv[cont], "r");
		// Comprobamos que no este vacio
		if (fichero == NULL) {
			printf ("UVacat: no puedo abrir fichero\n");
			exit(1);
		}	
		// Mostramos cada linea del archivo
		while (fgets(buffer, 255, fichero) != NULL){
			printf("%s", buffer);
		}
		// Pasamos al siguiente archivo
		cont++;
		fclose(fichero);
	}
	return (0);
}
