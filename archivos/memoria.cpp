#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

//HEAP: reserva infinita
void consumirHeap() {
    vector<char*> bloques;

    while (true) {
        char* bloque = new char[1024 * 1024]; // 1 MB
        memset(bloque, 0, 1024 * 1024);      // forzar uso real de memoria
        bloques.push_back(bloque);

        cout << "Reservado 1MB en heap. Total: " << bloques.size() << " MB" << endl;
    }
}

//STACK: recursión infinita
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
