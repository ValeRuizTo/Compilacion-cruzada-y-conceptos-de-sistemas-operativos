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

```bash
ls -lh
```

**Resultado:**

```
[COMPLETAR]
```

#### Dependencias dinámicas

```bash
ldd dinamico
ldd estatico
```

**Resultado:**

```
[COMPLETAR]
```

#### Trazabilidad de syscalls

```bash
strace -o traza_dinamica.txt ./dinamico
strace -o traza_estatica.txt ./estatico
diff traza_estatica.txt traza_dinamica.txt
```

**Análisis:**

* Syscalls adicionales en dinámico:

```
[COMPLETAR]
```

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
