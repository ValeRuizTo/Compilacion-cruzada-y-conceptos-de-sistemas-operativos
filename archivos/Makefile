# ========================
# CONFIGURACIÓN GENERAL
# ========================

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Para hilos
PTHREAD = -lpthread

# Para GPIO
GPIOD = -lgpiod

# Cross-compiler (opcional)
CROSS = aarch64-linux-gnu-g++
CROSS_FLAGS = -Wall -std=c++17

# ========================
# TARGETS PRINCIPALES
# ========================

all: hola dinamico estatico hilos procesos blink

# ========================
# HOLA MUNDO
# ========================

hola:
	$(CXX) hola.cpp -o hola

dinamico:
	$(CXX) hola.cpp -o hola_dinamico

estatico:
	$(CXX) hola.cpp -o hola_estatico -static

# ========================
# HILOS
# ========================

hilos:
	$(CXX) hilos.cpp -o hilos $(PTHREAD)

# ========================
# PROCESOS
# ========================

procesos:
	$(CXX) procesos.cpp -o procesos

# ========================
# GPIO (libgpiod)
# ========================

blink:
	$(CXX) blink.cpp -o blink $(GPIOD)

# ========================
# CROSS-COMPILING
# ========================

cross:
	$(CROSS) hola.cpp -o hola_arm_dinamico
	$(CROSS) hola.cpp -o hola_arm_estatico -static

# ========================
# LIMPIEZA
# ========================

clean:
	rm -f hola hola_dinamico hola_estatico hilos procesos blink hola_arm_dinamico hola_arm_estatico
