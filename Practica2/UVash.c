#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


// FUNCIONES //

// Imprime msj error por terminal	
void print_error() {

	char error_msj[30] = "An error has occurred\n";
	fprintf(stderr, "%s", error_msj); 
}


// Busca indx del char > en la cadena y devuelve
	// indx si solo hay un >
	// -1 si no hay ninguno
	// -2 si hay mas de uno
int busca_redireccion(char *tokens[], int n) {

	int indx = -1;
	int cont = 0;

	for (int j = 0; j < n; j++) {

		if (strcmp(tokens[j], ">") == 0) {
			indx = j;
			cont++;
		}
	}

	if (cont == 0) return -1;		// Sin redireccion
	if (cont > 1) return -2;		// Multiples >
	return indx;					// Exactamente 1, devolvemos su posicion
}




// MAIN //

int main (int argc, char *argv[]) {
	

	char *linea = NULL;
	size_t capacidad = 0;
	ssize_t leidos;
	
	FILE *entrada = stdin;
	int modo = 1;		// 1 == Modo batch ; 0 == Modo interactivo

	// Comprobamos si se ha pasado un archivo
	if (argc == 2) {
		entrada = fopen(argv[1], "r");
		if (entrada == NULL) {					// ERROR -> No se puede entrar al modo batch - EXIT 1
			print_error();
			exit(1);
		}
		modo = 0;	// Apagamos el modo batch
	
	} else if (argc > 2) { 						// ERROR: Demasiados argumentos al principio - EXIT 1
		print_error();
		exit(1);
	}




	// Bucle del UVash
	while(1){
		
		linea = NULL;


		// Comprobamos el modo de ejecucion y leemos del lugar adecuado
		if (modo == 1) {
			printf("UVash >");
			fflush(stdout);
		}	

		// Leemos segun el modo de ejecucion
		leidos = getline(&linea, &capacidad, entrada);

		

		// Para que no se queje gcc a la hora de compilar
		if (leidos == -1) {
			free(linea);
			exit(0);
		}
		
			

		// Limpiamos el salto de linea final
		linea[strcspn(linea, "\n")] = '\0';	
			

		
		// 3. Valido que si quiere salir del UVash
		if (strcmp(linea, "exit") == 0) {
			exit(0);
		}	



		// 2. Procesamos la entrada por termial
		char *tokens[128];			// Aqui guardamos la entrada troceada por tokens
		int i = 0;					// Contador
		char *token; 				// Puntero auxiliar para la entrada por terminal
		char *punt_linea = linea;		

		// Separamos por espacios y tabulaciones
		while ((token = strsep(&punt_linea, " \t")) != NULL) {
			// Validamos que no sea una palabra vacia
			if (strlen(token) > 0) {
				tokens[i] = token;
				i++;
			}
		}
		// El ultimo elemento debe ser NULL
		tokens[i] = NULL;	


		// REDIRECCIONES
		char *fich_salida = NULL;		// Nombre archivo salida
		int error_redir = 0;				// Error en la redireccion

		int redir_index = busca_redireccion(tokens, i);

		if (redir_index == -2) {
			// mas de un >
			error_redir = 1; 
		} else if (redir_index != -1) {
			if (tokens[redir_index + 1] == NULL) {
				// No hay nada despues del >
				error_redir = 1;
			} else if (tokens[redir_index + 2] != NULL) {
				// Hay mas de un archivo 
				error_redir = 1;
			} else {
				//Caso Valido
				fich_salida = tokens[redir_index + 1];
				tokens[redir_index] = NULL;
			}
		} 

		if (error_redir) {
			print_error();
			free(linea);
			continue;
		}




		// 5. Built-in para el comando [cd]
		if (strcmp(tokens[0], "cd") == 0) {
			// Tiene que tener exactamente 1 argimento
			if (tokens[1] == NULL || tokens [2] != NULL) {
				print_error();
			} else {
			// Intentamos el cambio de directorio y en caso no exitoso, error
				if (chdir(tokens[1]) != 0) {
					print_error();
				}
			}
			continue;
		}
		

		// 4. Hacemos las ejecuciones de los comandos
		pid_t pid = fork();		// Clonamos el proceso actual del shell

		if (pid < 0) {
			print_error();
		
		// Hijo, se ejecuta el comando o comandos
		} else if (pid == 0) {

			// Aplicamos redirecciones
			if (fich_salida != NULL) {
				// Abrimos fichero en modo
				int fich_redir = open(fich_salida, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fich_redir < 0) {
					print_error();
					exit(1);
				} 

				dup2(fich_redir, 1);
				dup2(fich_redir, 2);
				close(fich_redir);
			}

			// Ejecutamos los comandos
			execvp(tokens[0], tokens);

			print_error();
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



