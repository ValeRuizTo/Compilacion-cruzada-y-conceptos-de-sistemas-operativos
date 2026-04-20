# Compilación cruzada y conceptos de sistemas operativos

# Observabilidad del Sistema y Sistemas Embebidos

Repositorio del proyecto práctico enfocado en:

* Enlazado estático vs dinámico
* Cross-compiling
* Interacción con hardware (GPIO)
* Manejo de tiempo, memoria y concurrencia en Linux

---

# Parte A: Observabilidad del Sistema

##  Actividad 1: Enlazado Estático vs Dinámico

### Compilación

<img width="598" height="68" alt="image" src="https://github.com/user-attachments/assets/3054c93d-3777-4c20-af99-d701249d90fe" />


g++ hola_mundo.cpp -o dinamico
g++ hola_mundo.cpp -o estatico -static

<img width="600" height="167" alt="image" src="https://github.com/user-attachments/assets/e11e9f17-dfb7-43d7-b4f2-87387358b807" />



### Análisis

<img width="847" height="152" alt="image" src="https://github.com/user-attachments/assets/93c6f055-b647-4a53-9a80-580bd2444828" />

### Tamaño de binarios

- Binario dinámico: 17 KB  
- Binario estático: 2.3 MB  

El binario estático es aproximadamente **135 veces más grande** que el dinámico.

Esto ocurre porque el enlazado estático incluye todas las librerías dentro del ejecutable, mientras que el dinámico solo referencia librerías externas.

<img width="742" height="182" alt="image" src="https://github.com/user-attachments/assets/1415fcb5-a278-4a0b-90ce-56a957e74196" />

### Diferencia de tamaño

De la salida:

hola_dinamico → 17K
hola_estatico → 2.3M

#### Proporción exacta
2.3 MB ≈ 2300 KB
2300 / 17 ≈ 135 veces más grande


### Dependencias dinámicas

El binario dinámico requiere las siguientes librerías del sistema:

- libstdc++.so.6 (librería estándar de C++)
- libc.so.6 (librería estándar de C)
- libm.so.6 (operaciones matemáticas)
- libgcc_s.so.1 (soporte del compilador)
- ld-linux-x86-64.so.2 (loader dinámico)
- linux-vdso.so.1 (optimización de syscalls)

En cambio, el binario estático no requiere dependencias externas, ya que todas están incluidas dentro del ejecutable.

### Trazabilidad de llamadas al sistema

Cantidad de syscalls:

- Binario dinámico: 66 llamadas
- Binario estático: 19 llamadas

El binario dinámico realiza muchas más llamadas al sistema debido a la carga de librerías en tiempo de ejecución.

Las llamadas adicionales más relevantes son:

- openat → apertura de librerías compartidas
- mmap → mapeo de librerías en memoria
- mprotect → configuración de permisos de memoria
- close → cierre de archivos abiertos

```bash

access("/etc/ld.so.preload", R_OK)
openat(..., "/etc/ld.so.cache", ...)
openat(..., "/lib/x86_64-linux-gnu/libstdc++.so.6", ...)
openat(..., "/lib/x86_64-linux-gnu/libc.so.6", ...)
openat(..., "/lib/x86_64-linux-gnu/libm.so.6", ...)
```
En particular, se identifican los siguientes grupos de syscalls:

- **Acceso a archivos**: `access`, `openat`  
  → Utilizadas para localizar y abrir las librerías dinámicas (.so)

- **Lectura de archivos**: `read`, `pread64`  
  → Permiten cargar el contenido de las librerías

- **Gestión de memoria**: `mmap`  
  → Se usa para mapear las librerías en el espacio de memoria del proceso

- **Permisos de memoria**: `mprotect`  
  → Configura permisos de lectura y ejecución sobre las regiones cargadas

- **Metadatos de archivos**: `fstat`  
  → Obtiene información sobre las librerías

Estas llamadas no aparecen en la versión estática, ya que todas las dependencias están incluidas dentro del binario, eliminando la necesidad de cargarlas dinámicamente.
---

## Actividad 2: Cross-Compiling

Compilar un programa sencillo (Hola Mundo dinámico) para arquitectura ARM64 (aarch64) desde la máquina host (VirtualBox), verificar su arquitectura, ejecutarlo con emulación y finalmente transferirlo y ejecutarlo de forma nativa en la Raspberry Pi 5.

### Codigo fuente

```
#include <iostream>

int main() {
    std::cout << "Hola Mundo" << std::endl;
    return 0;
}

```
Cross-Compilación en la máquina Host

```
aarch64-linux-gnu-g++ hola.cpp -o hola_arm_dinamico
```

Verificación de la arquitectura del binario
```
file hola_arm_dinamico
```

<img width="826" height="120" alt="image" src="https://github.com/user-attachments/assets/5deafa62-c498-481d-8a87-4369b1a70938" />

Ejecución simulada en la máquina Host (usando QEMU)

```
qemu-aarch64 -L /usr/aarch64-linux-gnu/ ./hola_arm_dinamico
```
Transferencia del binario a la Raspberry Pi 5

```
scp hola_arm_dinamico raspi@10.89.220.87:/home/raspi/
```

<img width="1300" height="145" alt="image" src="https://github.com/user-attachments/assets/966e2b93-1195-4438-9d0a-00a89c20bccc" />

### Análisis

- Cross-compiling permite aprovechar la potencia de la máquina host (PC) para compilar más rápido.
- El comando file es muy útil para confirmar que el binario tiene la arquitectura correcta (aarch64 en lugar de x86_64).
- El binario es dinámicamente linked, por lo que necesita las librerías correspondientes en la Raspberry Pi (que ya vienen instaladas por defecto en Raspberry Pi OS).
- Usando scp se transfiere fácilmente el ejecutable a la placa.
- La ejecución nativa en la Raspberry Pi 5 es mucho más eficiente que emularla con QEMU.

# Parte B: Interacción con Hardware

## Actividad 3: GPIO con libgpiod

### Código fuente (blink.cpp)

```bash
#include <iostream>
#include <gpiod.h>
#include <unistd.h>

int main() {
    gpiod_chip *chip;
    gpiod_line *line;

    // Abrir el chip GPIO por su label (nombre)
    chip = gpiod_chip_open_by_name("gpiochip0");
    if (!chip) {
        std::cerr << "Error: No se pudo abrir gpiochip0" << std::endl;
        return 1;
    }

    // Obtener la línea 17 (GPIO17 - comúnmente usada para LEDs)
    line = gpiod_chip_get_line(chip, 17);
    if (!line) {
        std::cerr << "Error: No se pudo obtener la línea 17" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Solicitar la línea como salida
    if (gpiod_line_request_output(line, "blink", 0) < 0) {
        std::cerr << "Error: No se pudo configurar la línea como salida" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    std::cout << "Parpadeo iniciado en GPIO17. Presiona Ctrl+C para detener." << std::endl;

    while (true) {
        // Encender (ON)
        gpiod_line_set_value(line, 1);
        std::cout << "ON" << std::endl;
        sleep(1);

        // Apagar (OFF)
        gpiod_line_set_value(line, 0);
        std::cout << "OFF" << std::endl;
        sleep(1);
    }

    // Limpieza (nunca se alcanza por el bucle infinito)
    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
```

Compilación en la Raspberry Pi 5

```bash
g++ blink.cpp -o blink -lgpiod
```

Ejecución del programa
```bash
sudo ./blink
```
<img width="570" height="560" alt="image" src="https://github.com/user-attachments/assets/d42cfa27-e806-4af0-9d24-cd8aa85051cc" />


Verificación del estado del GPIO mientras el programa está corriendo
```
gpioinfo
```
<img width="1040" height="455" alt="image" src="https://github.com/user-attachments/assets/8b97dce9-6846-4584-ac6b-8ad68b3c8499" />

Compilación y ejecución (resumen en una sola captura)

<img width="588" height="140" alt="image" src="https://github.com/user-attachments/assets/e5dc90e8-ccad-4641-9e3d-3b60234a24ef" />

libgpiod es la forma de controlar GPIO en Linux.

- El programa reserva la línea GPIO17 como salida y la configura con el consumer name "blink".
- Mientras el programa está en ejecución, otros procesos pueden ver que la línea está ocupada mediante gpioinfo.

Es necesario ejecutar el programa con sudo porque el acceso a /dev/gpiochip* requiere permisos elevados.

**Observación:**
<img width="450" height="526" alt="image" src="https://github.com/user-attachments/assets/1e3014e2-ed89-4454-b724-01f3ec651138" />

<img width="409" height="488" alt="image" src="https://github.com/user-attachments/assets/2a6765ce-2c02-499f-9c9e-23187dbd8854" />


# Parte C: Tiempo, Memoria y Concurrencia

## Actividad 4: Delay con nanosleep

### Ejecución

<img width="697" height="452" alt="image" src="https://github.com/user-attachments/assets/09b603d7-2794-4f2e-b96d-ac3001844125" />



**Resultados:**

* real: 2.006 s
* user: 0.005 s
* sys: 0.000 s

**Análisis:**

Al ejecutar el programa con time, se observa que el tiempo real (real) es aproximadamente de 2 segundos, mientras que el tiempo de CPU en modo usuario (user) es muy cercano a 0 (0.005s).
Esto ocurre porque la función nanosleep suspende la ejecución del proceso durante el tiempo especificado, en lugar de realizar una espera activa. Durante este período, el proceso permanece bloqueado y no utiliza el procesador.
El pequeño valor en user corresponde únicamente a la ejecución de instrucciones previas y posteriores al delay, como la impresión en pantalla.
Durante el tiempo de espera, el control del procesador lo tiene el kernel del sistema operativo, el cual puede asignarlo a otros procesos o tareas, demostrando así la eficiencia del manejo de concurrencia en sistemas multitarea.

**¿Por qué el tiempo user es casi 0.000s? y ¿Quién tenía el control del procesador?**

Porque el programa no está usando la CPU activamente durante esos 2 segundos. Cuando se usa nanosleep el proceso le dice al kernel:

“duérmeme por 2 segundos” lo que hace que el proceso se bloquee (sleep state) y no ejecuta instrucciones por ende no consume CPU y esto causa que 
lo tiempo sean, en real aproximadamente≈ 2 segundos (tiempo de reloj) y en user aproximadamente 0 (CPU casi no trabajó)

Es decir que el tiempo user es cercano a 0.000s porque el programa no realiza una espera activa, sino que utiliza la llamada al sistema nanosleep, la cual suspende el proceso durante el tiempo especificado. Durante este periodo, el proceso no ejecuta instrucciones ni consume CPU, por lo que el tiempo de uso del procesador es mínimo. El control del procesador durante esos 2 segundos lo tiene el kernel del sistema operativo, que puede asignarlo a otros procesos o tareas mientras el programa permanece en estado de espera. El proceso no usa CPU porque está bloqueado; el kernel es quien maneja el procesador en ese tiempo.
 

---

## Actividad 5: Heap vs Stack

### Contexto

El espacio de direcciones de un proceso tiene límites definidos:

- **Heap**: maneja memoria dinámica (crece hacia arriba)
- **Stack**: maneja variables locales y llamadas a funciones (crece hacia abajo)

El objetivo es observar cómo se comportan ambos cuando se consumen de forma infinita.

---

##  Código utilizado

```cpp
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

//  HEAP: reserva infinita
void consumirHeap() {
    vector<char*> bloques;

    while (true) {
        char* bloque = new char[1024 * 1024]; // 1 MB
        memset(bloque, 0, 1024 * 1024);      // forzar uso real de memoria
        bloques.push_back(bloque);

        cout << "Reservado 1MB en heap. Total: " << bloques.size() << " MB" << endl;
    }
}

//  STACK: recursión infinita
void consumirStack(int profundidad = 0) {
    char bloque[1024 * 1024]; // 1 MB en stack
    memset(bloque, 0, sizeof(bloque));

    cout << "Profundidad: " << profundidad << " MB en stack" << endl;

    consumirStack(profundidad + 1);
}

int main() {
    int opcion;
    cout << "1. Heap infinito\n2. Stack infinito\nSeleccione: ";
    cin >> opcion;

    if (opcion == 1) consumirHeap();
    else consumirStack();

    return 0;
}
```
<img width="833" height="548" alt="image" src="https://github.com/user-attachments/assets/b5da422e-0692-4f99-ae7e-37ff59a70a60" />



## Consumo de Heap 

<img width="1188" height="572" alt="image" src="https://github.com/user-attachments/assets/0ae91e02-41c1-4d28-b9e3-ed245eb02774" />

Se ejecutó el programa y se monitoreó con htop. El proceso ./memoria (PID 7989) consumió rápidamente memoria, en htop se vio claramente cómo aumentaba la columna VIRT (memoria virtual) y especialmente RES (memoria residente). y el sistema llegó a un estado crítico (Load average alto, ~99% de uso de memoria).

- Evento OOM Killer (Out of Memory)
Después de que el sistema mató el proceso, se ejecutó:
```
sudo dmesg | tail -n 20
```

Resultado obtenido:
```
[ 2551.168614] oom-kill:constraint=CONSTRAINT_NONE,nodemask=(null),cpuset=/,mems_allowed=0,global_oom,task_memcg=/user.slice/user-1000.slice/user@1000.service/app.slice/app-org.gnome.Terminal.slice/vte-spawn-2e1aa72e-641b-4b08-92f7-f17e2c292543.scope,task=memoria,pid=9015,uid=1000
[ 2551.168639] Out of memory: Killed process 9015 (memoria) total-vm:5143420kB, anon-rss:2860800kB, file-rss:0kB, shmem-rss:0kB, UID:1000 pgtables:10112kB oom_score_adj:200
```
- El proceso llegó a reservar más de 5 GB de memoria virtual (total-vm: 5143420 kB).
- Se utilizaron aproximadamente 2.86 GB de memoria física real (anon-rss: 2860800 kB).
- Cuando el sistema se quedó sin memoria disponible, el OOM Killer intervino y terminó el proceso automáticamente.
Esto demuestra que el Heap puede crecer casi hasta agotar toda la memoria del sistema (física + swap).

## Consumo de Stack 

<img width="510" height="297" alt="image" src="https://github.com/user-attachments/assets/80e00913-38c0-443d-b98f-9379ef50e0e0" />

El error observado fue 
```
[ 2672.273180] memoria[9324]: segfault at 7fff7cebddd0 ip 000063d38b565410 sp 00007fff7cebddd0 error 6 in memoria[1410,63d38b565000+2000] likely on CPU 1 (core 1, socket 0)```

```

- El proceso falló con Segmentation Fault al alcanzar 6 MB de profundidad en el stack.
- Esto ocurrió porque el stack tiene un límite máximo fijo configurado por el sistema operativo
- Cada llamada recursiva + el arreglo local de 1 MB hace que el stack crezca muy rápido y supere este límite, ademas el Sistema Operativo detiene el proceso inmediatamente con un segfault para evitar que se corrompa otras áreas de memoria.

A diferencia del Heap, aquí no se llega al OOM porque el límite del stack es mucho más restrictivo y se hace cumplir antes.


## Actividad 6: Procesos vs Hilos

### Archivo de prueba

https://www.gutenberg.org/ebooks/2000.txt.utf-8

---

### Versión Procesos


```
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <map>

using namespace std;

int main() {
    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if (pid == 0) {
        //  HIJO: recibe palabras y cuenta
        close(fd[1]);

        map<string, int> contador;
        char buffer[100];

        while (read(fd[0], buffer, sizeof(buffer)) > 0) {
            string palabra(buffer);
            contador[palabra]++;
        }

        cout << "Conteo en hijo:\n";
        for (auto &p : contador) {
            cout << p.first << ": " << p.second << endl;
        }

        close(fd[0]);
    } else {
        //  PADRE: lee archivo y envía palabras
        close(fd[0]);

        FILE* file = fopen("libro.txt", "r");
        char palabra[100];

        while (fscanf(file, "%s", palabra) != EOF) {
            write(fd[1], palabra, sizeof(palabra));
        }

        fclose(file);
        close(fd[1]);

        wait(NULL);
    }

    sleep(10); // para observar en htop
    return 0;
}
```
<img width="792" height="232" alt="image" src="https://github.com/user-attachments/assets/ad8d7206-66f4-4faf-b605-2ae93d9c3774" />

- Se ejecutó la versión con fork() + pipe.
- Aparecen dos procesos distintos (./procesos).
- Tienen PIDs diferentes (1796 y 9948 en la captura).
- Cada proceso tiene su propio espacio de memoria.
La memoria RES (memoria residente física) no se duplica de forma idéntica porque el padre solo lee y envía, mientras que el hijo cuenta. Sin embargo, el código del programa y las bibliotecas se comparten parcialmente gracias al mecanismo de Copy-on-Write.

### Versión Hilos

```
#include <iostream>

#include <pthread.h>

#include <queue>

#include <map>

#include <fstream>

#include <string>

#include <unistd.h>
 
using namespace std;
 
queue<string> cola;

map<string, int> contador;
 
pthread_mutex_t mutex1;
 
bool terminado = false;
 
//  Productor (lee archivo)

void* leerArchivo(void*) {

    ifstream file("libro.txt");

    string palabra;
 
    while (file >> palabra) {

        pthread_mutex_lock(&mutex1);

        cola.push(palabra);

        pthread_mutex_unlock(&mutex1);
 
        usleep(500); // pequeño delay para hacerlo visible

    }
 
    terminado = true;

    return NULL;

}
 
//  Consumidor (cuenta palabras)

void* contar(void*) {

    while (true) { //  infinito para mantener el hilo vivo

        pthread_mutex_lock(&mutex1);
 
        if (!cola.empty()) {

            string palabra = cola.front();

            cola.pop();

            contador[palabra]++;

        }
 
        pthread_mutex_unlock(&mutex1);
 
        usleep(1000); // evita que consuma CPU al 100%

    }
 
    return NULL;

}
 
int main() {

    pthread_t t1, t2;
 
    pthread_mutex_init(&mutex1, NULL);
 
    pthread_create(&t1, NULL, leerArchivo, NULL);

    pthread_create(&t2, NULL, contar, NULL);
 
    //  NO hacemos join para que sigan vivos

    // pthread_join(t1, NULL);

    // pthread_join(t2, NULL);
 
    cout << "Hilos ejecutándose... revisa htop" << endl;
 
    sleep(30); //  tiempo suficiente para verlos en htop
 
    return 0;

}
 
```
<img width="912" height="152" alt="image" src="https://github.com/user-attachments/assets/cde1b576-ddf3-4a81-93d7-9cc47e109086" />

Se ejecutó la versión con pthread.
Captura con ps -eLf | grep hilos:

- Todos los hilos tienen el mismo PID principal (11443).
- Cada hilo tiene un TID (Thread ID) diferente (11443, 11444, 11445).
- Todos comparten el mismo TGID (Thread Group ID) = 11443.
- La memoria RES es exactamente la misma para todos los hilos porque comparten el mismo espacio de direcciones del proceso.

El PID que vemos en ps o htop para hilos es en realidad el TID (identificador del hilo).
Todos los hilos pertenecen al mismo grupo de hilos (TGID = PID del proceso principal).
Los hilos comparten la memoria (heap, datos globales, código), por eso la columna RES es prácticamente idéntica y no se duplica.


En sistemas Linux, cada hilo es una entidad planificable independiente, por lo que el sistema operativo le asigna un identificador propio (TID o PID a nivel de kernel). Esto explica por qué cada hilo aparece con un identificador diferente.Sin embargo, todos los hilos pertenecen a un mismo proceso, por lo que comparten el mismo TGID (Thread Group ID), el cual corresponde al identificador del proceso principal. Además, al pertenecer al mismo proceso, todos los hilos comparten el mismo espacio de direcciones de memoria, incluyendo el heap y las variables globales. Por esta razón, la memoria RES (memoria residente) es exactamente la misma para todos los hilos, ya que no existe duplicación de memoria como ocurre en el caso de los procesos.


### Análisis Final

| Característica        | Procesos                                      | Hilos                                      |
|----------------------|----------------------------------------------|-------------------------------------------|
| Espacio de memoria   | Aislado (cada uno tiene su propio)           | Compartido                                |
| Creación             | `fork()` (más pesado)                        | `pthread_create()` (más ligero)           |
| Comunicación         | Pipe, sockets, etc.                          | Variables globales + mutex                |
| Memoria RES          | No se comparte completamente                 | Se comparte (misma RES)                   |
| Identificador        | PID diferente                                | Mismo PID/TGID, TID diferente             |
| Overhead             | Mayor                                        | Menor                                     |
| Protección           | Alta (fallo en uno no afecta al otro)        | Baja (fallo en uno puede afectar a todos) |

---
