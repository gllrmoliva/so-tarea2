#ifndef TYPES_H
#define TYPES_H

// #################################################################
// En este archivo solo se define la estrura de los marcos de página
// #################################################################

typedef struct {
    unsigned long vpn; // virtual page number
    int valid;         // 1 (ocupado), 0 (free) 
    int ref_bit;       // bit de referencia para clock
} Frame;

#endif
