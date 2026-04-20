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

```bash
time ./delay
```

**Resultados:**

* real:
* user:
* sys:

**Análisis:**

```
[COMPLETAR]
```

---

## Actividad 5: Heap vs Stack

### Heap

* Monitoreo con `htop`
* Evento OOM:

```bash
dmesg | tail -n 20
```

**Resultado:**

```
[COMPLETAR]
```

### Stack

* Error observado:

```
Segmentation fault
```

**Profundidad alcanzada:**

```
[COMPLETAR]
```

**Análisis:**

```
[COMPLETAR]
```

---

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
