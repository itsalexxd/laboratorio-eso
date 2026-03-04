#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {

	char *linea = NULL;
	size_t capacidad = 0;
	ssize_t leidos;
	

	FILE *entrada = stdin;
	int modo = 1;		// Iniciamos el modo batch

	// Comprobamos si se ha pasado un archivo#
	if (argc == 2) {
		entrada = fopen(argv[1], "r");
		if (entrada == NULL) {
			fprintf(stderr, "%s", "[ERROR] -> No se ha podido abrir el modo batch...");
			exit(1);
		}
		modo = 0;	// Apagamos el modo batch
	
	} else if (argc > 2) {
		fprintf(stderr, "%s", "[ERROR] -> Ha ocurrido un error...");
		exit(1);
	}




	// Bucle del UVash
	while(1){
		
		linea = NULL;

		if (modo) {
			printf("UVash> ");
		}
				
		// 1. Leemos por terminal la entrada por teclado
		leidos = getline(&linea, &capacidad, stdin);
		

		

		// Para que no se queje gcc a la hora de compilar
		if (leidos == -1) {
			exit(0);
		}
		
			

		// Limpiamos el salto de linea final
		linea[strcspn(linea, "\n")] = '\0';	
			

		
		// 3. Valido que se quiere salir del UVashel
		if (strcmp(linea, "exit") == 0) {
			exit(0);
		}	



		// 2. Procesamos la entrada por termial
		char *tokens[128];		// Aqui guardamos los comandos troceados
		int i = 0;
		char *token; 			// Puntero auxiliar de tokens
		char *punt_linea = linea;		

		while ((token = strsep(&punt_linea, " \t")) != NULL) {
			// Validamos que no sea una palabra vacia
			if (strlen(token) > 0) {
				tokens[i] = token;
				i++;
			}
		}
		// El ultimo elemento debe ser NULL
		tokens[i] = NULL;	


		// 5. Built-in para el comando [cd]
		if (strcmp(tokens[0], "cd") == 0) {
			// Tiene que tener exactamente 1 argimento
			if (tokens[1] == NULL || tokens [2] != NULL) {
				fprintf(stderr, "%s", "-> [ERROR]: cd dir1/dir2/...\n\n");
			} else {
			// Intentamos el cambio de directorio y en caso no exitoso, error
				if (chdir(tokens[1]) != 0) {
					fprintf(stderr, "%s", "-> [ERROR]: No ha sido posible acceder al directorio...\n\n");
				}
			}

			continue;
		}
		

		// 4. Hacemos las ejecuciones de los comandos
		pid_t pid = fork();		// Clonamos el proceso actual del shell

		if (pid < 0) {
			printf("-> Error a la hora de clonar el shell");
		
		// Hijo, se ejecuta el comando o comandos
		} else if (pid == 0) {
			execvp(tokens[0], tokens);

			fprintf(stderr, "%s", "-> Error a la hora de ejecutar el comando en el hijo...\n");
			exit(1);


		// Padre, se queda en espera hasta que termine el hijo
		} else {
			wait(NULL);
		}

	
		free(linea);
	}

	// free(linea);
	return 0;

}
