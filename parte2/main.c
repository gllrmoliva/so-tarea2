#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source/pager.h"

// ######################################################
//  En este archivo está el flujo principal del programa.
// ######################################################

int main(int argc, char *argv[]) {

    // nota help
    if (argc < 4) {
        fprintf(stderr, "Uso: %s [N_marcos] [tamaño_marco] [--verbose] traza.txt\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n_frames = atoi(argv[1]);
    int page_size = atoi(argv[2]);
    char *filename = NULL;
    int verbose_mode = 0;
    
    // parseo de args
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "--verbose") == 0) {
            verbose_mode = 1;
        } else {
            filename = argv[i];
        }
    }

    if (!filename) {
        fprintf(stderr, "Error: falta el archivo de traza.\n");
        return EXIT_FAILURE;
    }

    // inicializar simulador
    pager_init(n_frames, page_size);
    // en caso de verbose activar
    pager_set_verbose(verbose_mode);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error abriendo archivo.");
        pager_cleanup();
        return EXIT_FAILURE;
    }

    //  proceso de addreses en cada línea de archivo de texto
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        unsigned long addr = strtoul(line, NULL, 0); 
        pager_process_address(addr);
    }

    pager_print_report();

    fclose(file);
    pager_cleanup();
    
    return EXIT_SUCCESS;
}
