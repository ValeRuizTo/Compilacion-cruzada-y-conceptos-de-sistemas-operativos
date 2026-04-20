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
        // HIJO: recibe palabras y cuenta
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
        // PADRE: lee archivo y envía palabras
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
