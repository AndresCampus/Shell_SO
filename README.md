# Shell_SO
Ficheros fuente base para el desarrollo de la práctica de SO del Grado en Ingeniería Informática de la UMA

## Listado de ficheros:
- ***Shell_project.c***: fichero fuente con el esqueleto donde desarrollar el Shell.
- ***job_control.c***: fichero fuente con la implementación de funciones básicas de soporte para el desarrollo del Shell (ver documentación en el guión de la práctica).
- ***job_control.h***: fichero con definiciones/cabeceras de las funciones de soporte.
- ***redirections.c***: fichero fuente con ejemplo de redirección de entrada y salida estándar.
- ***example_list.c***: fichero fuente con ejemplo de uso de la API de lista de trabajos (jobs).
- ***Makefile***: fichero para automatizar la compilación del proyecto y los ejemplos.
---
### Extras:
- ***.devcontainer***: directorio con la configuración de un contenedor para la extensión Dev Container de VSCode con las herramientas necesarias para desarrollar la práctica. Es necesario tener instalado Docker-Desktop (Windows o Mac) y la extensión "Dev Container" en VSCode. 
- ***.vscode***: directorio con ficheros para poder compilar y depurar el proyecto directamente en VSCode (para shell tipo Unix con compilador gcc instalado)
---

### Compilación:
Para compilar tanto el Shell como los ejemplos:
`make`

Para compilar un objetivo específico:
`make Shell`, `make redirections` o `make example_list`

Para limpiar los ejecutables:
`make clean`

### Ejecución:
`./Shell`

Para salir pulsar ^D

---
### Uso del ejemplo de redirecciones:
Este ejemplo permite ejecutar un comando (ej. `sort`) redirigiendo su entrada desde un fichero y su salida a otro.

Para probarlo con el fichero `entrada.txt` proporcionado:
1. Compilar: `make redirections`
2. Ejecutar: `./redirections entrada.txt salida.txt sort`
---
### Uso del ejemplo de lista de trabajos:
Este ejemplo demuestra cómo manipular la lista de trabajos (añadir, buscar, borrar y listar) usando la API de `job_control.h`.

Para probarlo:
1. Compilar: `make example_list`
2. Ejecutar: `./example_list`

---
## Descripción del módulo ***job_control***

- ### get_command
      void get_command(char inputBuffer[], int size, char *args[], int *background);
    Devuelve en ***args*** el array de argumentos del comando leído, por ejemplo para el comando ***ls –l***, tendremos ***args[0]=”ls”***, ***args[1]=”-l”*** y ***args[2]=NULL***. La variable ***background*** se pondrá a 1 (true) si el comando termina con &. Necesita como entrada un buffer de caracteres y su tamaño para trabajar sobre él (***inputBuffer*** y ***size***). Hay un ejemplo de uso en Shell_project.c.

---
- ### job_state, state_strings
      enum job_state {FOREGROUND, BACKGROUND, STOPPED};
      static char* state_strings[] = {"Foreground","Background","Stopped"};
    Este es un tipo enumerado para identificar el estado actual de un trabajo en la lista de trabajos. Se ha definido un array de cadenas para permitir su impresión por pantalla de forma más directa, por ejemplo, siendo ***estado*** una variable de tipo ***job_state*** podríamos informar de su valor así:

      printf("Estado del trabajo: %s\n", state_strings[estado]);

---
- ### job
      typedef struct job_
      {
        pid_t pgid;           /* group id = process leader id */
        char * command;       /* program name */
        enum job_state state;
        struct job_ *next;    /* next job in the list */
      } job;

    El tipo ***job*** se usa para representar las tareas y enlazarlas en la lista de tareas. Una lista de tareas será un puntero a ***job (job \*)*** y un nodo de la lista se manejará también como un puntero a ***job (job \*)***.

---
- ### new_job
      job * new_job(pid_t pid, const char * command, enum job_state state);
    Para crear una nueva tarea. Se proporciona como entrada a la función el ***pid***, el nombre del comando y el estado (BACKGROUND o FOREGROUND).

---
- ### new_list, list_size, empty_list, print_job_list
      job * new_list(const char * name);
      int list_size(job * list);
      int empty_list(job *  list);
      void print_job_list(job * list);
    Estas funciones se utilizan para crear una nueva lista (con una cadena como nombre), para averiguar el tamaño de la lista, para comprobar si la lista está vacía (devuelve 1 si está vacía) y para pintar el contenido de una lista de tareas por pantalla. En realidad, están implementadas como macros.

---
- ### add_job, delete_job, get_item_bypid, get_item_bypos
      void add_job(job * list, job * item);
      int delete_job(job * list, job * item);
      job* get_item_bypid(job * list, pid_t pid);
      job* get_item_bypos(job * list, int n);
    Estas funciones trabajan sobre una lista de tareas. Las dos primeras añaden o eliminan una tarea a la lista (***delete_job*** devuelve 1 si la eliminó correctamente). Las dos últimas buscan y devuelven un elemento de la lista por pid o por posición. Devolverán NULL si no lo encuentran. Un ejemplo de uso de ***add_job*** podría ser:

      job * my_job_list = new_list(“Tareas Shell”);
      ...
      add_job(my_job_list, new_job(pid_fork, args[0], background) );

---
- ### ignore_terminal_signals, restore_terminal_signals
      void ignore_terminal_signals()
      void restore_terminal_signals()
    Estas funciones (en realidad macros) se utilizan para desactivar o activar las señales relacionadas con el terminal (***SIGINT, SIGQUIT, SIGTSTP, SIGTTIN, SIGTTOU***). El shell debe ignorar estas señales, pero el comando creado con ***fork()*** debe restaurar su comportamiento por defecto.

---
- ### block_SIGCHLD, unblock_SIGCHLD
      void block_SIGCHLD()
      void unblock_SIGCHLD()
    Estas macros nos son de utilidad para bloquear la señal ***SIGCHLD*** en las secciones de código donde el shell modifique o acceda a estructuras de datos (la lista de tareas) que pueden ser accedidas desde el manejador de esta señal y así evitar riesgos de acceso a datos en estados no válidos o no coherentes.

---
- ### parse_redirections
      void parse_redirections(char **args, char **file_in, char **file_out)
    Esta función permite obtener los nombres de ficheros correspondientes a las redirecciones simples (operadores > y <). Esta función ha de ser llamada inmediatamente después a la invocación de ***get_command()***. Las cadenas por referencia, ***file_in*** y ***file_out***, contienen los nombres del fichero asociado a las redirecciones < y > respectivamente, siendo NULL si dicha redirección no está presente.

---
## Guía de Pruebas Sugeridas

Para verificar que el Shell funciona correctamente, se recomienda seguir este orden de pruebas:

### 1. Comandos básicos (Foreground)
- `ls`
- `ps`
- `ls -l /usr/bin`
- `cat Shell_project.c`

### 2. Comandos en segundo plano (Background)
- `sleep 10 &` (El shell debe devolver el control inmediatamente)
- `jobs` (Debería aparecer el proceso sleep en la lista)

### 3. Redirecciones 
- `ls -l > salida.txt`
- `sort < salida.txt`
- `grep "main" < Shell_project.c > lineas_main.txt`

### 4. Comandos internos (Built-in)
- `cd /tmp`
- `cd ..`
- `jobs`
- `fg 1` (Pasar a primer plano la tarea 1)
- `bg 1` (Reanudar en segundo plano la tarea parada 1)

### 5. Control de señales
- Ejecutar `sleep 100` y pulsar `Ctrl+C`: El proceso debe terminar y el Shell debe seguir vivo.
- Ejecutar `sleep 100` y pulsar `Ctrl+Z`: El proceso debe pararse, el Shell debe informar de ello y volver al prompt.