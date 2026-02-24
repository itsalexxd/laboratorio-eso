#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    // REGLA 1: Si no se pasan argumentos
    if (argc < 2) {
        printf("UVagrep: searchterm [file ...]\n");
        return 1;
    }

    // Guardamos nuestro término de búsqueda
    char *search_term = argv[1];

    // Variables para getline
    char *linea = NULL;
    size_t capacidad = 0;

    // REGLA 2: Si hay término de búsqueda, pero NO hay archivos
    // Significa que argc es exactamente 2. Debemos leer de "stdin"
    if (argc == 2) {
        while (getline(&linea, &capacidad, stdin) != -1) {
            // Comprobamos si el término está en la línea
            if (strstr(linea, search_term) != NULL) {
                printf("%s", linea);
            }
        }
    } 
    // REGLA 3: Si hay archivos, leemos archivo por archivo
    else {
        for (int i = 2; i < argc; i++) {
            FILE *archivo = fopen(argv[i], "r");
            
            // REGLA 4: Si no se puede abrir el archivo
            if (archivo == NULL) {
                printf("UVagrep: cannot open file\n");
                free(linea);
                return 1;
            }

            // Leemos el archivo línea por línea
            while (getline(&linea, &capacidad, archivo) != -1) {
                // Si el término está en la línea, la imprimimos
                if (strstr(linea, search_term) != NULL) {
                    printf("%s", linea);
                }
            }
            
            fclose(archivo);
        }
    }

    // Liberamos la memoria que reservó getline
    free(linea);

    // REGLA 5: En cualquier otro caso, salir con 0 (éxito)
    return 0;
}
