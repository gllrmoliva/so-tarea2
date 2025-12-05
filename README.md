# Tarea 2 - Sistemas Operativos

Solución para la Tarea 2 que abarca primitivas de sincronización y manejo de memoria virtual.

## Integrantes
* Guillermo Oliva Orellana
* Matías Olivas Henríquez

## Instrucciones:

El repositorio esta dividido en dos partes independientes:

### Parte 1: Barrera Reutilizable

El código compila utilizando `gcc` con la bandera `-pthread` requerida:

```bash
cd parte1
gcc -o barrera main.c -pthread
```

Se ejecuta parametrizando el número de hebras y etapas (5 y 4 por defecto):

```bash
./barrera [N_hebras] [E_etapas]
```

### Parte 2: Simulador de Memoria Virtual
