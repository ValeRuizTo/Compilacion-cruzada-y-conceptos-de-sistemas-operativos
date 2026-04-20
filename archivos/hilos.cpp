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

    // NO hacemos join para que sigan vivos
    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);

    cout << "Hilos ejecutándose... revisa htop" << endl;

    sleep(30); // tiempo suficiente para verlos en htop

    return 0;
}
