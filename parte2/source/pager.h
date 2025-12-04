#ifndef PAGER_H
#define PAGER_H

//############################
// declaracion de funciones de paginación
//############################

/*
inicializa las estructuras del simulador.
 - frames: número de marcos físicos
 - size: tamaño de página en bytes (OJO: debe ser potencia de 2)
*/
void pager_init(int frames, int size);

/*
activa el verbose mode (1 activado, 0 desactivado)
*/
void pager_set_verbose(int v);

/*
procesa una dirección virtual, el input es la dirección
*/
void pager_process_address(unsigned long virtual_addr);

void pager_print_report();

/*
libera la memoria.
*/
void pager_cleanup();

#endif
