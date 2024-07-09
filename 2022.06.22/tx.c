/*
Scrivere due programmi C, tx e rx: tx deve trasferire a rx stringhe di max 8 caratteri usando i valori
assegnati ai segnali (il parametro value della funzione sigqueue).
Il programma rx deve per prima cosa stampare il proprio pid e attendere segnali.
ill programma tx ha due parametri, il pid did rx e il messaggio.
es: tx 22255 sigmsg
(posto che rx sia in esecuzione con pid 22255, rx deve stampare sigmsg)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig, siginfo_t *info, void *ucontext) {
    char c = info->si_value.sival_int; // Estrae il carattere inviato
    write(STDOUT_FILENO, &c, 1); // Stampa il carattere
    if (c == '\0') { // Se è il carattere di terminazione, esce
        write(STDOUT_FILENO, "\n", 1);
        exit(0);
    }
}

int main() {
    printf("PID: %d\n", getpid());

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = signal_handler;
    sigaction(SIGUSR1, &sa, NULL);

    while (1) pause(); // Attende i segnali

    return 0;
}