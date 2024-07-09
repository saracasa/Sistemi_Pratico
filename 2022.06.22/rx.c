#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <PID>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);

    // Invia il segnale SIGUSR1 al processo con PID specificato
    if (kill(pid, SIGUSR1) == -1) {
        perror("Errore nell'invio del segnale");
        return 1;
    }

    return 0;
}