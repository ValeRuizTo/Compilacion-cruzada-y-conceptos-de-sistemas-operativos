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

```bash
g++ hola_mundo.cpp -o dinamico
g++ hola_mundo.cpp -o estatico -static
```

### Análisis

#### Tamaño de binarios

- Binario dinámico: 17 KB  
- Binario estático: 2.3 MB  

El binario estático es aproximadamente **135 veces más grande** que el dinámico.

Esto ocurre porque el enlazado estático incluye todas las librerías dentro del ejecutable, mientras que el dinámico solo referencia librerías externas.

#### Diferencia de tamaño

De la salida:

hola_dinamico → 17K
hola_estatico → 2.3M

#### Proporción exacta
2.3 MB ≈ 2300 KB
2300 / 17 ≈ 135 veces más grande


#### Dependencias dinámicas

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

Estas llamadas son necesarias para que el loader dinámico pueda cargar las dependencias antes de ejecutar el programa principal.

En contraste, el binario estático no necesita cargar librerías externas, por lo que ejecuta menos llamadas al sistema.
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
