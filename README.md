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

Para compilar el simulador:

```bash
cd parte2
gcc -o sim main.c
````

La ejecución requiere especificar el número de marcos, el tamaño de página (en bytes) y el archivo de traza. Opcionalmente acepta la bandera `--verbose` para mostrar el paso a paso de la traducción.

**Sintaxis:**

```bash
./sim Nmarcos tamañomarco [--verbose] traza.txt
```

**Ejemplos de uso:**

1.  Ejecución simple (solo reporte final):

    ```bash
    ./sim 8 4096 traza.txt
    ```

2.  Ejecución detallada (con traducción paso a paso):

    ```bash
    ./sim 16 4096 --verbose traza.txt
    ```
