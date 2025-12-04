#include <stdio.h>
#include <stdlib.h>
#include "pager.h"
#include "types.h"

//##################
// Simulación
//##################


// variables de simulación
static Frame *physical_memory = NULL;   // array que simula memoria física
static int n_frames;                    // cantidad de frames 
static int page_size;                   // tamaño de paginación
static int shift_bits;                  // b
static unsigned long mask;              // MASK
static int clock_hand = 0;              // pointer de reloj
static int verbose = 0;                 // indica modo de print

// para impresión de estadisticas
static unsigned long total_refs = 0;
static unsigned long page_faults = 0;

// calcula el valor de b (log_2(size))
static int get_shift_bits(int size) {
    int b = 0;
    // shifteamos 1 b veces hasta que 2^b >= size
    while ((1 << b) < size) {
        b++;
    }
    // si b > size, size no es potencia de dos (entonces muere)
    // si b = size, es potencia de dos wiiii
    if ((1 << b) != size) {
        fprintf(stderr, "Error: El tamaño de página (%d) debe ser potencia de 2.\n", size);
        exit(EXIT_FAILURE);
    }
    return b;
}

// algoritmo clock a.k.a. juez holden
static int get_victim_frame() {
    while (1) {
    // se puede ocupar el espacio porque está libre
        if (!physical_memory[clock_hand].valid) {
            return clock_hand;
        }

    // si está ocupado
        // si fuiste ocupado hace poco, copero, seras la victima
        if (physical_memory[clock_hand].ref_bit == 0) {
            int victim = clock_hand;
            clock_hand = (clock_hand + 1) % n_frames;
            return victim;
        // si no fuiste ocupado hace poco, te salvaste..., si te veo en 0...
        } else {
            physical_memory[clock_hand].ref_bit = 0;
            clock_hand = (clock_hand + 1) % n_frames;
        }
    }
}

// buscar si la página está en memoria
static int lookup_page(unsigned long vpn) {
    for (int i = 0; i < n_frames; i++) {
        if (physical_memory[i].valid && physical_memory[i].vpn == vpn) {
            return i;
        }
    }
    return -1;
}

// inicializar, calcular variables de utilidad
void pager_init(int frames, int size) {
    n_frames = frames;
    page_size = size;
    shift_bits = get_shift_bits(page_size); // esto es b en enunciado
    mask = page_size - 1;
    
    physical_memory = (Frame *)calloc(n_frames, sizeof(Frame)); // reservar en heap (con 0's)
    if (!physical_memory) {
        perror("Error: algo fallo reservando memoria para frames");
        exit(EXIT_FAILURE);
    }
}

// verbosidad setter!
void pager_set_verbose(int v) {
    verbose = v;
}

void pager_process_address(unsigned long virtual_addr) {

    total_refs++;
    
    unsigned long offset = virtual_addr & mask;     // offset como en enunciado
    unsigned long vpn = virtual_addr >> shift_bits; // vpn como en enunciado
    
    int frame_index = lookup_page(vpn);             // ver si está en frame
    int is_fault = 0;
    
    if (frame_index != -1) {
        // HIT: si existe en memoria, ponemos su ref en 1 (para clock)
        physical_memory[frame_index].ref_bit = 1;
    } else {
        // MISS: no está en memoria...
        is_fault = 1;
        page_faults++;
        
        // revisar si queda algún frame libre (frag. ext.)
        // si queda genial
        // si no....
        int found_free = 0;
        for (int i = 0; i < n_frames; i++) {
            if (!physical_memory[i].valid) {
                frame_index = i;
                found_free = 1;
                break;
            }
        }
        
        // si no buscamos una victima
        if (!found_free) {
            frame_index = get_victim_frame();
        }
        
        // actualizar mapeo
        physical_memory[frame_index].vpn = vpn;
        physical_memory[frame_index].valid = 1;
        physical_memory[frame_index].ref_bit = 1;
    }
    
    // calculo de memoria física
    unsigned long physical_addr = ((unsigned long)frame_index << shift_bits) | offset;
    
    if (verbose) {
        printf("DV: 0x%lx\t NPV: 0x%lx\t Offset: 0x%lx\t %s\t Marco: %d\t DF: 0x%lx\n",
               virtual_addr, vpn, offset, is_fault ? "MISS" : "HIT ", frame_index, physical_addr);
    }
}

void pager_print_report() {
    printf("\n--- Resultados ---\n");
    printf("Tamaño Marco: %d bytes, Marcos Totales: %d\n", page_size, n_frames);
    printf("Referencias totales: %lu\n", total_refs);
    printf("Fallos de página:    %lu\n", page_faults);
    if (total_refs > 0) {
        printf("Tasa de fallos:      %.4f%%\n", (double)page_faults / total_refs * 100.0);
    }
}

// liberar memoria
void pager_cleanup() {
    if (physical_memory) {
        free(physical_memory);
        physical_memory = NULL;
    }
}
