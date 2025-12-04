#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/**
 * Implementación de Barrera
 */

/* Estructura de barrera reutilizable implementada como monitor */
typedef struct {
    pthread_mutex_t mutex; // exclusión mutua para el estado interno
    pthread_cond_t cond;   // condición de espera
    int count;             // hebras que han llegado
    int N;                 // hebras totales
    int etapa;             // etapa
} Barrera;

/* Inicializa la barrera */
void barrier_init(Barrera *b, int N) {
    b->N = N;
    b->count = 0;
    b->etapa = 0;
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cond, NULL);
}

/* Destructor */
void barrier_destroy(Barrera *b) {
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->cond);
}

/* Bloquea la barrera hasta que llegan N hebras. Reutilizable */
void barrier_wait(Barrera *b) {
    
    // exclusión mutua antes de acceder al estado interno del monitor
    pthread_mutex_lock(&b->mutex);

    int mi_etapa = b->etapa; // hay que guardar la etapa a la que pertenece la hebra
    b->count++;

    /* Caso de que no sea la última hebra */
    if (b->count < b->N) {

        // si ya no es la etapa a la que pertenece la hebra, salir
        // acá se evita el problema de la Hebra Rápida
        while (mi_etapa == b->etapa) {
            // 1. liberamos el mutex, 2. esperamos el broadcast de la variable de condición
            pthread_cond_wait(&b->cond, &b->mutex);
        }
    /* Caso de que sea la última hebra */
    } else {
        // paso a la nueva etapa
        b->etapa++;
        b->count = 0;
        pthread_cond_broadcast(&b->cond);
    }

    // liberar el lock antes de salir
    pthread_mutex_unlock(&b->mutex);
}

/**
 * Testing
 */

/* Estructura para pasar argumentos a las hebras */
typedef struct {
    int id;
    int etapas_totales;
    Barrera *barrera;
} Args;

/* Función que ejectua cada hebra */
// espera un tiempo aleatorio
// imprime antes del wait
// imprime despues del wait
void* mi_thread(void *args) {
    Args *data = (Args*)args;
    int id = data->id;
    int etapas_totales = data->etapas_totales;
    Barrera *barrera = data->barrera;

    for (int e = 0; e < etapas_totales; e++) {
        
        usleep(rand() % 200000); // hasta 0.2 segundos de trabajo

        printf("Hebra %d: \tesperando en etapa %d\n", id, e);

        barrier_wait(barrera);

        printf("Hebra %d: \tpasó barrera en etapa %d\n", id, e);
    }

    free(data);
    return NULL;
}

/* Lee los parametros y crea las hebras */
int main(int argc, char *argv[]) {

    // lectura de parametros
    int N = 5;
    int E = 4;
    if (argc > 1) N = atoi(argv[1]);
    if (argc > 2) E = atoi(argv[2]);

    printf("Iniciando con N=%d hebras y E=%d etapas\n\n", N, E);

    srand(time(NULL)); // semilla

    Barrera barrera;
    barrier_init(&barrera, N);

    pthread_t threads[N];

    /* Creación de hebras */
    for (int i = 0; i < N; i++) {

        Args *args = malloc(sizeof(Args));
        args->id = i;
        args->etapas_totales = E;
        args->barrera = &barrera;

        if (pthread_create(&threads[i], NULL, mi_thread, args) != 0) {
            perror("Error creando hebra");
            exit(1);
        }
    }

    /* Esperar a que todas las hebras terminen */
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    barrier_destroy(&barrera);
    printf("\nEjecución exitosa.\n");

    return 0;
}