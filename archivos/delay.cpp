#include <iostream>
#include <time.h>

void delay(int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;              // segundos
    ts.tv_nsec = (ms % 1000) * 1000000; // nanosegundos

    nanosleep(&ts, NULL);
}

int main() {
    std::cout << "Inicio" << std::endl;

    delay(2000); // 2 segundos

    std::cout << "Fin" << std::endl;
    return 0;
}
