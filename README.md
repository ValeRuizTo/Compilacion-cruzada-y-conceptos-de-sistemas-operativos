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

### Compilación

```bash
aarch64-linux-gnu-g++ hola_mundo.cpp -o hola_arm
```

### Verificación

```bash
file hola_arm
```

**Salida:**

```
[COMPLETAR]
```

### Ejecución con QEMU

```bash
qemu-aarch64 -L /usr/aarch64-linux-gnu/ ./hola_arm
```

### Transferencia a Raspberry Pi

```bash
scp hola_arm pi@<IP>:/home/pi/
```

---

# Parte B: Interacción con Hardware

## Actividad 3: GPIO con libgpiod

### Compilación cruzada

```bash
aarch64-linux-gnu-g++ gpio_blink.cpp -o gpio_blink -lgpiod
```

### Verificación en Raspberry Pi

```bash
gpioinfo
gpioget
```

**Observación:**

```
[COMPLETAR]
```

---

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

<img width="833" height="548" alt="image" src="https://github.com/user-attachments/assets/b5da422e-0692-4f99-ae7e-37ff59a70a60" />

El error observado fue 
```
[ 2672.273180] memoria[9324]: segfault at 7fff7cebddd0 ip 000063d38b565410 sp 00007fff7cebddd0 error 6 in memoria[1410,63d38b565000+2000] likely on CPU 1 (core 1, socket 0)```

```

- El proceso terminó con un Segmentation Fault (segfault).
- Esto ocurrió porque el stack tiene un límite máximo fijo (generalmente 8 MB en Ubuntu).
Cada llamada recursiva + el arreglo local de 1 MB hace que el stack crezca muy rápido y supere este límite, ademas el Sistema Operativo detiene el proceso inmediatamente con un segfault para evitar que se corrompa otras áreas de memoria.

A diferencia del Heap, aquí no se llega al OOM porque el límite del stack es mucho más restrictivo y se hace cumplir antes.

<img width="510" height="297" alt="image" src="https://github.com/user-attachments/assets/80e00913-38c0-443d-b98f-9379ef50e0e0" />

El proceso falló con Segmentation Fault al alcanzar 6 MB de profundidad en el stack.
Esto ocurre porque el stack tiene un tamaño máximo fijo configurado por el sistema operativo


## Actividad 6: Procesos vs Hilos

### Archivo de prueba

https://www.gutenberg.org/ebooks/2000.txt.utf-8

---

### Versión Procesos

* Observación en `top`:

```
[COMPLETAR]
```

* Uso de memoria:

```
[COMPLETAR]
```

---

### Versión Hilos

* Activar hilos en `top`:

```
H
```

* Observaciones:

```
[COMPLETAR]
```

---

### Análisis Final

* Diferencia entre PID y TGID:

```
[COMPLETAR]
```

* Uso de memoria compartida:

```
[COMPLETAR]
```

---
