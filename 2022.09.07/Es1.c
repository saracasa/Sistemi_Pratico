/*CHAT giusto???
Scrivere un programma C rilancia: che esegua il programma indicato in argv[1] con i relativi parametri
(in argv[2] e seguenti):
es: rilancia tr a-z A-Z
esegue il comando tr coi parametri.
Se il programma lanciato termina senza errori (non per colpa di un segnale e con valore di ritorno 0)
rilancia deve rieseguire il programma (nell'esempio tr) con i medesimi parametri.
*/

// tr sostituisce i caratteri minuscoli con i corrispondenti maiuscoli

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int status; //stato del processo figlio
    
    if (argc < 2) {
        printf("Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();
        
    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) { 
        execvp(argv[1], &argv[1]); 
        perror("execvp");
        return 1;
    } else {
        wait(&status);
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        // Il processo figlio è terminato correttamente
        printf("Il comando è stato eseguito con successo.\n");
        execvp(argv[1], &argv[1]);
    } else {
        // Gestione dell'errore o del caso in cui il processo figlio non termina correttamente
        printf("Il comando non è terminato correttamente.\n");
    }
    }
    
    return 0;
}
