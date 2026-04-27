#include <stdio.h>
#include <stdlib.h>
#include "job_control.h"

/**
 * Programa de ejemplo para mostrar el uso de la API de gestión de trabajos (jobs)
 * definida en job_control.h.
 * Este programa es útil para que los estudiantes entiendan cómo manipular
 * la lista de procesos en segundo plano del shell.
 */

int main() {
    // 1. Crear una lista nueva llamada "mi_lista"
    // La macro new_list crea un nodo centinela (cabecera) para la lista.
    job * mi_lista = new_list("mi_lista");

    printf("--- Ejemplo de gestión de lista --- \n\n");

    // Uso de la macro empty_list para comprobar si la lista no tiene elementos
    if (empty_list(mi_lista)) {
        printf("La lista '%s' está inicialmente vacía.\n", mi_lista->command);
    }

    // 2. Añadir un par de elementos:
    // {1001, "ls", "Foreground"}, {1002, "sleep", "Background"}
    // Usamos new_job para crear el nodo y add_job para insertarlo en la lista.
    
    printf("Añadiendo trabajos a la lista...\n");
    job * work1 = new_job(1001, "ls", FOREGROUND);
    add_job(mi_lista, work1);

    job * work2 = new_job(1002, "sleep", BACKGROUND);
    add_job(mi_lista, work2);

    // Uso de la macro list_size para obtener el número de elementos
    printf("Trabajos añadidos. Tamaño actual de la lista: %d\n", list_size(mi_lista));

    // 3. Pintar la lista completa usando la macro print_job_list
    printf("\nEstado actual de la lista:\n");
    print_job_list(mi_lista);

    // 4. Buscar por PID el 1001 y pintarlo
    printf("\nBuscando trabajo con PID 1001...\n");
    job * found = get_item_bypid(mi_lista, 1001);
    
    if (found != NULL) {
        printf("Trabajo encontrado: ");
        print_item(found);

        // 5. Borrar el elemento encontrado
        printf("\nBorrando el trabajo 1001...\n");
        delete_job(mi_lista, found);
    } else {
        printf("No se encontró el trabajo con PID 1001.\n");
    }

    // 6. Volver a pintar la lista entera tras el borrado
    printf("\nLista tras el borrado (Tamaño: %d):\n", list_size(mi_lista));
    print_job_list(mi_lista);

    printf("\n--- Fin del ejemplo ---\n");

    return 0;
}
